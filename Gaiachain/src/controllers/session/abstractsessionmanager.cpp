#include "abstractsessionmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

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

void AbstractSessionManager::onTokenChanged(const QString &token)
{
    m_token = token;
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
    auto newState = (errorCode == QNetworkReply::NoError) ? Enums::ConnectionState::ConnectionSuccessful
                                                          : Enums::ConnectionState::ConnectionError;
    if (m_connectionState == newState) {
        return;
    }

    m_connectionState = newState;
    emit connectionStateChanged(newState);
}
