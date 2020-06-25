#include "replantationssender.h"

#include "common/tags.h"
#include "common/logs.h"
#include "rest/baserequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "database/dbhelpers.h"
#include "charcoal/database/charcoaldbhelpers.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QJsonObject>

ReplantationsSender::ReplantationsSender(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT id, plotId, userId, numberOfTrees, treeSpecies, "
               "locationLatitude, locationLongitude, beginningDate, endingDate "
               "FROM Replantations WHERE isCommitted=0");

    connect(this, &ReplantationsSender::refreshed,
            this, &ReplantationsSender::sendEvents);
}

void ReplantationsSender::sendEvents()
{
    const int count = rowCount();
    if (count == 0) {
        return;
    }

    const bool isLoggedIn = m_userManager->isLoggedIn();
    for (int i = 0; i < count; ++i) {
        // use query().next()?
        if (query().seek(i) == false) {
            continue;
        }

        const auto record = query().record();
        qDebug() << "Offline replantation is:" << record;

        const auto request = QSharedPointer<BaseRequest>::create(
            "/entities/replantation/",
            BaseRequest::Type::Post
            );

        const QJsonObject location({
            { "latitude", query().value("locationLatitude").toDouble() },
            { "longitude", query().value("locationLongitude").toDouble() }
        });

        // We have to read all data first, and only then we can call
        // getWebPackageId() because it invalidates current query()
        const auto numberOfTrees(query().value("numberOfTrees").toString());
        const qint64 beginning = static_cast<qint64>(
            query().value("beginningDate").toLongLong());
        const qint64 ending = static_cast<qint64>(
            query().value("endingDate").toLongLong());
        const int treeSpecies = query().value("treeSpecies").toInt();
        const QString parentId(query().value("plotId").toString());
        const int webPlotId(CharcoalDbHelpers::getWebPackageId(m_connectionName, parentId));

        const QJsonDocument doc(
            {
                { "trees_planted", numberOfTrees },
                { Tags::webBeginningDate, beginning },
                { Tags::webEndingDate, ending },
                { "location", location },
                { "plot", webPlotId },
                { Tags::webTreeSpecies, treeSpecies }
            });

        request->setDocument(doc);

        if (isLoggedIn) {
            qDebug() << "Sending replantation event!" << doc;
            request->setToken(m_sessionManager->token());
            m_sessionManager->sendRequest(request, this,
                                          &ReplantationsSender::webErrorHandler,
                                          &ReplantationsSender::webReplyHandler);
        } else {
            qDebug() << "Enqueuing replantation event!" << doc;
            m_queuedRequests.append(request);
        }
    }
}

void ReplantationsSender::webErrorHandler(const QString &errorString,
                                          const QNetworkReply::NetworkError code)
{
    qDebug() << "Request error!" << errorString << code;
    emit error(errorString);
}

void ReplantationsSender::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Request success!" << reply;

    const qint64 begin(reply.object().value(Tags::webBeginningDate).toInt());
    const qint64 end(reply.object().value(Tags::webEndingDate).toInt());
    const QString queryString(
        QString("UPDATE Replantations SET isCommitted=1 "
                "WHERE beginningDate=%1 AND endingDate=%2").arg(begin).arg(end));

    QSqlQuery query(queryString, db::Helpers::databaseConnection(m_connectionName));
    if (query.exec() == false) {
        const QLatin1String errorString = QLatin1String("Query to update the Replantation has failed to execute");
        qWarning() << RED(errorString)
                   << query.lastError() << "For query:" << query.lastQuery()
                   << reply;
        emit error(errorString);
    }
}
