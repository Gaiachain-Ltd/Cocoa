#include "destinationsmodel.h"

#include "rest/additionaldatarequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "listupdater.h"

#include <QSqlQuery>

DestinationsModel::DestinationsModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT name FROM Destinations");
}

void DestinationsModel::refreshWebData()
{
    const auto request = QSharedPointer<AdditionalDataRequest>::create(
        AdditionalDataRequest::DataType::Destinations);

    if (m_userManager->isLoggedIn()) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &DestinationsModel::webErrorHandler,
                                      &DestinationsModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequests.append(request);
    }
}

void DestinationsModel::webReplyHandler(const QJsonDocument &reply)
{
    ListUpdater updates("Destinations", m_connectionName);
    if (updates.updateTable(reply, "name")) {
        emit webDataRefreshed();
    } else {
        qWarning() << RED("Updating items has failed");
    }
}