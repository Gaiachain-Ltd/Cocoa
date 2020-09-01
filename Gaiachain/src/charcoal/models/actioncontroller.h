#pragma once

#include "common/enums.h"
#include "dbobjects.h"

#include <QObject>
#include <QPointer>
#include <QVariant>

class QGeoCoordinate;
class QSqlQuery;
class QQmlApplicationEngine;

class PicturesManager;

class BagsMatch
{
    Q_GADGET

    Q_PROPERTY(bool fullMatch MEMBER fullMatch)
    Q_PROPERTY(QVariantList missingBags MEMBER missingBags)
    Q_PROPERTY(QVariantList extraBags MEMBER extraBags)

public:
    BagsMatch() = default;
    ~BagsMatch() = default;

    /*!
     * Is set to true if bags from transport and reception stages match
     * completely.
     */
    bool fullMatch = false;

    /*!
     * List of QR codes which are present in Transport stage but missing from
     * Reception.
     */
    QVariantList missingBags;

    /*!
     * List of QR codes which are present in Reception but are not present in
     * Transport.
     */
    QVariantList extraBags;

    /*!
     * List of bags scanned in Loading and Transport step.
     */
    QVariantList bagsFromTransport;

    /*!
     * List of bags scanned in Reception step.
     */
    QVariantList bagsFromReception;

    Q_INVOKABLE QString matchStatusMessage() const;
};

Q_DECLARE_METATYPE(BagsMatch)

class ActionController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(OvenDimensions emptyOvenDimensions READ emptyOvenDimensions CONSTANT)

public:
    explicit ActionController(QObject *parent = nullptr);

    void setDbConnection(const QString &connectionName);
    void setPicturesManager(PicturesManager *manager);

    Q_INVOKABLE QString generatePlotId(const QString &userId,
                                       const QString &parcelCode,
                                       const QDate &date) const;
    Q_INVOKABLE QString generateHarvestId(const QString &plotId,
                                          const QString &userId) const;
    Q_INVOKABLE QString generateTransportId(const QString &harvestId,
                                            const QString &licensePlate,
                                            const int transportNumber,
                                            const QDate &date) const;

    Q_INVOKABLE bool plotExists(const QString &userId,
                                const QString &parcelCode,
                                const QDate &date);

    Q_INVOKABLE QString getPlotId(const QString &packageId);

    Q_INVOKABLE int getTransportIdFromBags(const QVariantList &scannedQrs,
                                           const bool silent = false) const;
    Q_INVOKABLE QString getEntityName(const int id) const;

    Q_INVOKABLE int nextTransportNumber(const int harvestId,
                                        const bool isPausedEvent) const;
    Q_INVOKABLE int bagCountInTransport(const int transportId) const;
    Q_INVOKABLE BagsMatch matchBags(const int transportId,
                                    QVariantList qrsFromReception);

    Q_INVOKABLE QString plateNumberInTransport(const int transportId) const;
    Q_INVOKABLE int scannedBagsCount(const int transportId) const;
    Q_INVOKABLE int scannedBagsTotal(const int transportId) const;
    Q_INVOKABLE int registeredTrucksCount(const int transportId) const;
    Q_INVOKABLE int registeredTrucksTotal(const int transportId) const;

    Q_INVOKABLE QString nextOvenNumber(int parentEntityId) const;
    Q_INVOKABLE OvenDimensions defaultOvenDimensions(const int ovenId) const;
    OvenDimensions emptyOvenDimensions() const;

    Q_INVOKABLE qreal ovenVolume(const qreal width,
                                 const qreal height,
                                 const qreal length) const;
    Q_INVOKABLE qreal ovenVolume(const qreal width,
                                 const qreal height1,
                                 const qreal height2,
                                 const qreal length) const;

    Q_INVOKABLE void registerLoggingBeginning(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const QString &parcel,
        const int parcelId,
        const int villageId,
        const int treeSpeciesId
        ) const;

    Q_INVOKABLE void registerLoggingEnding(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const int plotId,
        const int numberOfTrees
        ) const;

    Q_INVOKABLE void registerCarbonizationBeginning(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const QString &plotId,
        const int plotDbId,
        const QString &ovenId,
        const int ovenIdNumber,
        const OvenDimensions &ovenDimensions
        ) const;

    Q_INVOKABLE void registerCarbonizationEnding(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const int harvestId,
        const QVariantList &ovenIds
        ) const;

    Q_INVOKABLE void registerLoadingAndTransport(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const QString &transportId,
        const int harvestId,
        const QString &plateNumber,
        const QString &destination,
        const int destinationId,
        const QVariantList &scannedQrs,
        const bool pauseEvent
        ) const;

    Q_INVOKABLE bool registerReception(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const int transportId,
        const QVariantList &documents,
        const QVariantList &receipts,
        const QVariantList &scannedQrs
        ) const;

    Q_INVOKABLE void finalizeSupplyChain(const int transportId) const;

    Q_INVOKABLE void registerReplantation(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QString &userId,
        const int plotId,
        const int numberOfTrees,
        const int treeSpeciesId,
        const QDateTime &beginningDate,
        const QDateTime &endingDate
        ) const;

signals:
    void refreshLocalEvents() const;

    void finalizePackages(const QVector<int> &webIds) const;

    void error(const QString &text) const;

private:
    int scannedBagsForAction(const int transportId,
                             const Enums::SupplyChainAction action) const;

    bool insertEntity(QSqlQuery *query, const int typeId,
                      const QString &packageId,
                      const int parentId = -1) const;
    bool insertEvent(QSqlQuery *query,
                     const int entityId,
                     const int eventTypeId,
                     const QString &userId,
                     const QDateTime &timestamp,
                     const QDateTime &eventDate,
                     const QGeoCoordinate &coordinate,
                     const QVariantMap &properties,
                     const bool pauseEvent = false) const;
    bool updateEvent(QSqlQuery *query,
                     const int eventId,
                     const int entityId,
                     const int eventTypeId,
                     const QString &userId,
                     const QDateTime &timestamp,
                     const QDateTime &eventDate,
                     const QGeoCoordinate &coordinate,
                     const QVariantMap &properties,
                     const bool pauseEvent = false) const;

    const QString dateFormat = "dd-MM-yyyy";

    QString m_connectionName;
    QPointer<PicturesManager> m_picturesManager;
};

