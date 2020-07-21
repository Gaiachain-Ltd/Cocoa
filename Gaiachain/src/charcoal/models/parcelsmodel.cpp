#include "parcelsmodel.h"

#include "helpers/requestshelper.h"
#include "database/dbhelpers.h"
#include "rest/additionaldatarequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "common/tags.h"
#include "listupdater.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QSqlQuery>
#include <QSqlError>

ParcelsModel::ParcelsModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT code FROM Parcels");
}

void ParcelsModel::refreshWebData()
{
    const auto request = QSharedPointer<AdditionalDataRequest>::create(
        AdditionalDataRequest::DataType::Parcels);

    if (RequestsHelper::isOnline(m_sessionManager.get(), m_userManager.get())) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &ParcelsModel::webErrorHandler,
                                      &ParcelsModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequests.append(request);
    }
}

void ParcelsModel::getUnusedParcels()
{
    const auto request = QSharedPointer<BaseRequest>::create(
        "/additional_data/parcels/unused/",
        BaseRequest::Type::Get);

    if (RequestsHelper::isOnline(m_sessionManager.get(), m_userManager.get())) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &ParcelsModel::webErrorHandler,
                                      &ParcelsModel::webUnusedParcelsReplyHandler);
    } else {
        qDebug() << "Enqueing unused parcels request";
        m_queuedRequests.append(request);
    }
}

void ParcelsModel::webReplyHandler(const QJsonDocument &reply)
{
    ListUpdater updates("Parcels", m_connectionName);
    if (updates.updateTable(reply, Tags::code)) {
        // The unused parcels information is not used :D
        // Reason: parcels can be reused freely, see:
        // https://projects.milosolutions.com/issues/87745
        getUnusedParcels();
        return;
    } else {
        qWarning() << RED("Updating items has failed");
    }
}

void ParcelsModel::webUnusedParcelsReplyHandler(const QJsonDocument &reply)
{
    const QJsonObject mainObject(reply.object());
    const QJsonArray mainArray(mainObject.value("results").toArray());
    QVector<int> unusedParcelIds(mainArray.size());

    for (const QJsonValue &item : mainArray) {
        const QJsonObject object(item.toObject());
        unusedParcelIds.append(object.value(Tags::id).toInt());
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
    for (const int id : unusedParcelIds) {
        query.prepare("UPDATE Parcels SET isUsed=0 WHERE id=:id");
        query.bindValue(":id", id);
        if (query.exec() == false) {
            qWarning() << RED("Unable to update unused Parcel")
                       << "SQL error:" << query.lastError()
                       << "For query:" << query.lastQuery();
        }
    }

    emit webDataRefreshed();
}
