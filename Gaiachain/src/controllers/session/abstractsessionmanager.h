#ifndef ABSTRACTSESSIONMANAGER_H
#define ABSTRACTSESSIONMANAGER_H

#include "../abstractmanager.h"
#include "../../common/enums.h"

#include <QNetworkReply>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(sessionManager)

class AbstractSessionManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(Enums::ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)

public:
    explicit AbstractSessionManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    Enums::ConnectionState connectionState() const;

    Q_INVOKABLE void getInitialData();

    Q_INVOKABLE virtual void ping() = 0;

    Q_INVOKABLE virtual void login(const QString &email, const QString &password) = 0;

    Q_INVOKABLE virtual void getAdditionalData() = 0;

    Q_INVOKABLE virtual void getRelations(const QString &id) = 0;
    Q_INVOKABLE virtual void getRelations(const QStringList &ids) = 0;
    Q_INVOKABLE virtual void addRelation(const QString &id, const QStringList &ids) = 0;

    Q_INVOKABLE virtual void getEntitiesInfo(int count, const QDateTime &from = {}) = 0;
    Q_INVOKABLE virtual void getEntitiesInfo(const QDateTime &from, const QDateTime &to) = 0;
    Q_INVOKABLE virtual void getEntities(const QStringList &ids) = 0;
    Q_INVOKABLE virtual void getEntity(const QString &id) = 0;
    Q_INVOKABLE virtual void getEntityId(const QByteArray &codeData) = 0;
    Q_INVOKABLE virtual void putEntityAction(const QString &id, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                             const QVariantMap &properties, const QByteArray &codeData = {}) = 0;
    Q_INVOKABLE virtual void putEntityAction(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                                             const QDateTime &timestamp, const QVariantMap &properties) = 0;
    Q_INVOKABLE virtual void postNewEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                           const QVariantMap &properties, const QByteArray &codeData) = 0;

    Q_INVOKABLE virtual void getCreatedHarvestIds() = 0;

    Q_INVOKABLE virtual void getUnusedLotIds() = 0;
    Q_INVOKABLE virtual void postUnusedLotId() = 0;

    void updateToken(const QString &token);

signals:
    void connectionStateChanged(Enums::ConnectionState connectionState);

    void pingError(const int &code) const;
    void pingSuccess() const;

    void loginError(const int &code) const;
    void loginFinished(const QJsonDocument &doc) const;

    void additionalDataLoadError(const int &code) const;
    void additionalDataLoaded(const QJsonObject &data) const;

    void relationsLoadError(const int &code) const;
    void relationsLoaded(const QJsonArray &relations) const;
    void relationsSaveError(const int &code) const;
    void relationsSaved(const QString &id) const;

    void entitiesLoadError(const int &code) const;
    void entitiesInfoLoaded(const QJsonArray &entitiesInfo) const;
    void entitiesLoaded(const QJsonArray &entities) const;

    void entityIdLoadError(const int &code) const;
    void entityIdLoaded(const QString &id) const;

    void entitySaveError(const int &code) const;
    void entitySaved(const QString &id) const;

    void createdHarvestIdsLoadError(const int &code) const;
    void createdHarvestIdsLoaded(const QJsonArray &ids) const;

    void unusedLotIdsLoadError(const int &code) const;
    void unusedLotIdsLoaded(const QJsonArray &ids) const;
    void unusedLotIdCreateError(const int &code) const;
    void unusedLotIdCreated(const QString &id) const;

protected:
    Enums::ConnectionState m_connectionState = Enums::ConnectionState::Unknown;
    QString m_token;

    bool checkValidToken() const;

    void updateConnectionStateBeforeRequest();
    void updateConnectionStateAfterRequest(const QNetworkReply::NetworkError &errorCode = QNetworkReply::NoError);
};

#endif // ABSTRACTSESSIONMANAGER_H
