#include "trackingupdater.h"

#include "charcoal/database/charcoaldbhelpers.h"
#include "charcoal/picturesmanager.h"
#include "database/dbhelpers.h"
#include "common/logs.h"
#include "common/tags.h"

#include <QVariant>
#include <QFileInfo>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QSqlQuery>
#include <QSqlError>

TrackingUpdater::TrackingUpdater(const QString &connectionName)
    : m_connectionName(connectionName)
{
    if (isValid() == false) {
        qWarning() << RED("Invalid table name or connection name!")
                   << m_connectionName;
    }
}

void TrackingUpdater::setPicturesManager(PicturesManager *manager)
{
    m_picturesManager = manager;
}

bool TrackingUpdater::updateTable(const QJsonDocument &json) const
{
    const QJsonObject mainObject(json.object());
    const QJsonArray mainArray(mainObject.value("results").toArray());

    // We go back-to-front, in order to register oldest events first
    for (int i = mainArray.size() - 1; i >= 0; --i) {
        if (processTrackingItem(mainArray.at(i).toObject()) == false) {
            return false;
        }
    }

    return true;
}

bool TrackingUpdater::updateDetails(const QJsonDocument &json) const
{
    const QJsonObject package(json.object());
    const int webId(package.value("id").toInt(-1));
    const QString pid(package.value(Tags::pid).toString());
    const QJsonObject properties(package.value(Tags::properties).toObject());

    qDebug() << "Updating Package number" << webId << pid;

    bool result = true;
    for (const QString &key : properties.keys()) {
        if (key == "logging_beginning") {
            if (processDetailsLoggingBeginning(properties.value(key).toObject()) == false) {
                result = false;
            }
        } else if (key == "logging_ending") {
            if (processDetailsLoggingEnding(properties.value(key).toObject()) == false) {
                result = false;
            }
        } else if (key == "ovens") {
            if (processDetailsOvens(properties.value(key).toArray()) == false) {
                result = false;
            }
        } else if (key == "loading_transport") {
            if (processDetailsLoadingAndTransport(
                    pid, properties.value(key).toObject()) == false) {
                result = false;
            }
        } else if (key == "reception") {
            if (processDetailsReception(properties.value(key).toObject()) == false) {
                result = false;
            }
        }
    }

    return result;
}

bool TrackingUpdater::isValid() const
{
    return m_connectionName.isEmpty() == false;
}

bool TrackingUpdater::processTrackingItem(const QJsonObject &object) const
{
    const int webId(object.value("id").toInt(-1));
    const QString pid(object.value(Tags::pid).toString());
    const QString webType(object.value("type").toString());
    const Enums::PackageType type = packageType(webType);
    const int typeId(CharcoalDbHelpers::getEntityTypeId(m_connectionName, type));
    const bool isReplanted(object.value("plot_has_replantation").toBool(false));

    // Not parsed and not needed: "type_display" property
    const QJsonArray events(object.value("entities").toArray());

    int entityId = CharcoalDbHelpers::getEntityIdFromWebId(
        m_connectionName, webId, false);

    // Set proper parent
    int parentEntityId = -1;
    if (type == Enums::PackageType::Plot) {
        parentEntityId = 0;
    } else if (type == Enums::PackageType::Harvest) {
        const QString ppid(CharcoalDbHelpers::getPlotName(pid));
        parentEntityId = CharcoalDbHelpers::getEntityIdFromName(
            m_connectionName, ppid, false);
    } else if (type == Enums::PackageType::Transport) {
        const QString ppid(CharcoalDbHelpers::getHarvestName(pid));
        parentEntityId = CharcoalDbHelpers::getEntityIdFromName(
            m_connectionName, ppid, false);
    }

    if (entityId == -1) {
        // Entity does not exist in our DB - add it!
        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
        query.prepare("INSERT INTO Entities (typeId, name, parent, webId, "
                      "isFinished, isReplanted) "
                      "VALUES (:typeId, :name, :parent, :webId, 0, :isReplanted)");
        query.bindValue(":typeId", typeId);
        query.bindValue(":name", pid);
        if (parentEntityId != -1) {
            query.bindValue(":parent", parentEntityId);
        } else {
            query.bindValue(":parent", 0);
        }
        query.bindValue(":webId", webId);
        query.bindValue(":isReplanted", int(isReplanted));

        if (query.exec() == false) {
            qWarning() << RED("Inserting Entity has failed!")
                       << query.lastError().text()
                       << "for query:" << query.lastQuery();
            return false;
        }

        entityId = query.lastInsertId().toInt();
    }

    for (int i = events.size() - 1; i >= 0; --i) {
        const QJsonObject event(events.at(i).toObject());
        const int eventWebId(event.value("id").toInt(-1));
        const qint64 timestamp = event.value(Tags::timestamp).toVariant().toLongLong();
        const qint64 eventDate = event.value(Tags::webEventDate).toVariant().toLongLong();
        const QJsonArray location = event.value("location_display").toArray();
        const int eventTypeId = CharcoalDbHelpers::getEventTypeId(
            m_connectionName, event.value("action").toString());
        const QString userId(event.value("user_code").toString());

        const int eventId = CharcoalDbHelpers::getEventIdFromWebId(
            m_connectionName, eventWebId, false);

        if (eventId == -1) {
            QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
            query.prepare("INSERT INTO Events (entityId, typeId, userId, "
                          "webId, parentWebId, "
                          "date, eventDate, locationLatitude, locationLongitude, "
                          "properties, isCommitted) "
                          "VALUES (:entityId, :typeId, :userId, "
                          ":eventWebId, :parentWebId, "
                          ":date, :eventDate, :locationLatitude, :locationLongitude, "
                          ":properties, 1)");
            query.bindValue(":entityId", entityId);
            query.bindValue(":typeId", eventTypeId);
            query.bindValue(":userId", userId);
            query.bindValue(":eventWebId", eventWebId);
            query.bindValue(":parentWebId", webId);
            query.bindValue(":date", timestamp);
            query.bindValue(":eventDate", eventDate);
            query.bindValue(":locationLatitude", location.at(0).toDouble());
            query.bindValue(":locationLongitude", location.at(1).toDouble());

            if (query.exec() == false) {
                qWarning() << RED("Inserting event has failed!")
                           << query.lastError().text() << "for query:" << query.lastQuery();
                return false;
            }
        }
    }

    return true;
}

bool TrackingUpdater::processDetailsLoggingBeginning(const QJsonObject &object) const
{
    const QJsonObject webEntity(object.value("entity").toObject());
    const int webId = webEntity.value("id").toInt();
    const qint64 eventDate = object.value("beginning_date").toVariant().toLongLong();
    const int parcelId = object.value("parcel_id").toInt();
    const QString village(object.value("village").toString());
    const QString treeSpecies(object.value("tree_specie").toString());
    const int villageId = CharcoalDbHelpers::getVillageId(m_connectionName, village);
    const int treeSpeciesId = CharcoalDbHelpers::getTreeSpeciesId(
        m_connectionName, treeSpecies);

    return updateEventDetails(webId,
                              {
                                  { Tags::webParcel, parcelId },
                                  { Tags::webVillage, villageId },
                                  { Tags::webTreeSpecies, treeSpeciesId },
                                  { Tags::webEventDate, eventDate }
                              });
}

bool TrackingUpdater::processDetailsLoggingEnding(const QJsonObject &object) const
{
    const QJsonObject webEntity(object.value("entity").toObject());
    const int webId = webEntity.value("id").toInt();
    const qint64 eventDate = object.value("ending_date").toVariant().toLongLong();
    const int numberOfTrees = object.value("number_of_trees").toInt();

    return updateEventDetails(webId,
                              {
                                  { Tags::webNumberOfTrees, numberOfTrees },
                                  { Tags::webEventDate, eventDate }
                              });
}

bool TrackingUpdater::processDetailsOvens(const QJsonArray &array) const
{
    Q_UNUSED(array)

    return true;
}

bool TrackingUpdater::processDetailsLoadingAndTransport(const QString &packageName,
                                                        const QJsonObject &object) const
{
    const QString harvestName(CharcoalDbHelpers::getHarvestName(packageName));
    const int harvestEntity(CharcoalDbHelpers::getEntityIdFromName(
        m_connectionName, harvestName));
    const int webHarvestId(
        CharcoalDbHelpers::getWebPackageId(m_connectionName, harvestEntity));

    const QJsonObject webEntity(object.value("entity").toObject());
    const int webId = webEntity.value("id").toInt();
    const qint64 eventDate = object.value("loading_date").toVariant().toLongLong();
    const QString plateNumber = object.value("plate_number").toString();
    const int destinationId = object.value("destination_id").toInt();
    const QStringList scannedQrs(getQrCodes(object.value("bags").toArray()));

    return updateEventDetails(webId,
                              {
                                  { Tags::webHarvestId, webHarvestId },
                                  { Tags::webPlateNumber, plateNumber },
                                  { Tags::webDestination, destinationId },
                                  { Tags::webQrCodes, scannedQrs },
                                  { Tags::webEventDate, eventDate }
                              });
}

bool TrackingUpdater::processDetailsReception(const QJsonObject &object) const
{
    const QJsonObject webEntity(object.value("entity").toObject());
    const int webId = webEntity.value("id").toInt();
    const qint64 eventDate = object.value("reception_date").toVariant().toLongLong();

    const QStringList scannedQrs(getQrCodes(object.value("bags").toArray()));
    const QStringList docs(getImages(object.value("documents_photos").toArray()));
    const QStringList recs(getImages(object.value("receipt_photos").toArray()));

    return updateEventDetails(webId,
                              {
                                  { Tags::documents, docs },
                                  { Tags::receipts, recs },
                                  { Tags::webQrCodes, scannedQrs },
                                  { Tags::webEventDate, eventDate }
                              });
}

bool TrackingUpdater::updateEventDetails(const int webId,
                                         const QVariantMap &properties) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
    query.prepare("UPDATE Events SET properties=:properties "
                  "WHERE webId=:webId");
    query.bindValue(":properties", CharcoalDbHelpers::propertiesToString(properties));
    query.bindValue(":webId", webId);

    if (query.exec() == false) {
        qDebug() << RED("Updating details has failed")
                 << query.lastError().text()
                 << "for query:" << query.lastQuery();
        return false;
    }

    return true;
}

QStringList TrackingUpdater::getQrCodes(const QJsonArray &codes) const
{
    QStringList result;

    for (const auto &value : codes) {
        const QJsonObject bag(value.toObject());
        result.append(bag.value("qr_code").toString());
    }

    return result;
}

QStringList TrackingUpdater::getImages(const QJsonArray &images) const
{
    QStringList result;

    for (const auto &value : images) {
        const QJsonObject img(value.toObject());
        const QString path(img.value("image").toString());
        const QString fileName(QFileInfo(path).fileName());

        if (m_picturesManager) {
            m_picturesManager->checkFileIsCached(path);
        }

        result.append(fileName);
    }

    return result;
}

Enums::PackageType TrackingUpdater::packageType(const QString &webType) const
{
    if (webType == QLatin1String("TRK")) {
        return Enums::PackageType::Transport;
    } else if (webType == QLatin1String("HAR")) {
        return Enums::PackageType::Harvest;
    } else if (webType == QLatin1String("PLT")) {
        return Enums::PackageType::Plot;
    }

    return Enums::PackageType::Unknown;
}
