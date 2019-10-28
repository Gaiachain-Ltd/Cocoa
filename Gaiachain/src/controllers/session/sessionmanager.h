#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "abstractsessionmanager.h"
#include "../../rest/restapiclient.h"
#include "../../rest/baserequest.h"

class SessionManager : public AbstractSessionManager
{
    Q_OBJECT

public:
    explicit SessionManager(QObject *parent = nullptr);

    Q_INVOKABLE void login(const QString &email, const QString &password) override;

    Q_INVOKABLE void getAdditionalData() override;

    Q_INVOKABLE void getRelations(const QString &id) override;
    Q_INVOKABLE void addRelation(const QString &id, const QStringList &ids) override;

    Q_INVOKABLE void getEntitiesInfo(int count, const QDateTime &from = {}) override;
    Q_INVOKABLE void getEntitiesInfo(const QDateTime &to, const QDateTime &from = {}) override;
    Q_INVOKABLE void getEntities(const QStringList &ids) override;
    Q_INVOKABLE void getEntity(const QString &id) override;
    Q_INVOKABLE void getEntitId(const QByteArray &codeData) override;

    Q_INVOKABLE void putEntityAction(const QString &id, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                     const QVariantMap &properties, const QByteArray &codeData = {}) override;
    Q_INVOKABLE void putEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                               const QVariantMap &properties, const QByteArray &codeData) override;

    Q_INVOKABLE void getUnusedLotIds() override;
    Q_INVOKABLE void createUnusedLotId() override;

private:
    RestAPIClient m_client;

    void getAllRelations() override;
    void getAllEntities() override;

    void sendRequest(const QSharedPointer<BaseRequest> &request);
    void getEntities(const QJsonArray &ids);
};

#endif // SESSIONMANAGER_H
