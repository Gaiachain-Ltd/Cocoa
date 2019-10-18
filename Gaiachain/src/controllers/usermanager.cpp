#include "usermanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "../common/tags.h"
#include "../common/location.h"
#include "../common/dataglobals.h"
#include "../common/logs.h"

UserManager::UserManager(QObject *parent)
    : AbstractManager(parent)
{}

bool UserManager::isLoggedIn() const
{
    return m_userType != Enums::UserType::Annonymous;
}

QString UserManager::getLogin() const
{
    return isLoggedIn() ? m_userData[Tags::email].toString() : QString{};
}

void UserManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("userManager"), this);
}

void UserManager::skipLogin()
{
    setUserType(Enums::UserType::Annonymous);
}

void UserManager::logOut()
{
    setUserType(Enums::UserType::Annonymous);
    m_userData.clear();
}

Enums::UserType UserManager::getUserType() const
{
    return m_userType;
}

void UserManager::parseLoginData(const QJsonDocument &doc)
{
    const QJsonObject obj = doc.object();

    m_userData.insert(Tags::email, obj.value(Tags::email).toString());
    m_userData.insert(Tags::companyId, obj.value(Tags::companyId).toString());
    m_userData.insert(Tags::companyName, obj.value(Tags::companyName).toString());
    const QJsonArray locationArray = obj.value(Tags::companyLocation).toArray();
    Location location;
    location.lat = locationArray.at(0).toDouble();
    location.lon = locationArray.at(1).toDouble();
    m_userData.insert(Tags::companyLocation, QVariant::fromValue(location));
    const QString &role = obj.value(Tags::role).toString();
    const Enums::UserType userType = DataGlobals::userTypeFromString(role);

    emit tokenChanged(obj.value(Tags::token).toString());

    setUserType(userType);
    emit loginChanged(getLogin());
}

QVariantMap UserManager::userData() const
{
    return m_userData;
}

void UserManager::setUserType(const Enums::UserType userType)
{
    if (m_userType == userType)
        return;

    auto prevUserType = m_userType;
    m_userType = userType;
    emit userTypeChanged(userType);

    if (prevUserType == Enums::UserType::Annonymous ||
            userType == Enums::UserType::Annonymous) {
        emit loggedInChanged(isLoggedIn());
        emit loginChanged(getLogin());
    }
}
