#include "abstractsessionmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "../../helpers/requestshelper.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(sessionManager, "session.manager")

AbstractSessionManager::AbstractSessionManager(QObject *parent)
    : AbstractManager(parent)
{}

void AbstractSessionManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("sessionManager"), this);
}

Enums::ConnectionState AbstractSessionManager::connectionState() const
{
    return m_connectionState;
}

void AbstractSessionManager::getInitialData()
{
    getAdditionalData();
    getCreatedHarvestIds();
    getUnusedLotIds();
}

void AbstractSessionManager::updateToken(const QString &token)
{
    m_token = token;
}

bool AbstractSessionManager::checkValidToken() const
{
    if (m_token.isEmpty()) {
        qCWarning(sessionManager) << "Token is invalid!";
        return false;
    }
    return true;
}

void AbstractSessionManager::updateConnectionStateBeforeRequest()
{
    if (m_connectionState == Enums::ConnectionState::ConnectionSuccessful
            || m_connectionState == Enums::ConnectionState::Connecting) {
        return;
    }

    m_connectionState = Enums::ConnectionState::Connecting;
    emit connectionStateChanged(m_connectionState);
}

void AbstractSessionManager::updateConnectionStateAfterRequest(const QNetworkReply::NetworkError &errorCode)
{
    auto newState = (RequestsHelper::isNetworkError(errorCode) ||
                     RequestsHelper::isServerError(errorCode)) ? Enums::ConnectionState::ConnectionError
                                                               : Enums::ConnectionState::ConnectionSuccessful;
    if (m_connectionState == newState) {
        return;
    }

    m_connectionState = newState;
    emit connectionStateChanged(newState);
}
