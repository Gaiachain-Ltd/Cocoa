#include "pagemanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "../common/logs.h"

PageManager::PageManager(QObject *parent) : AbstractManager(parent)
{
    m_pageStack.append(m_initialPage);
}

void PageManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("pageManager"), this);
}

/*!
 * \brief PageManager::enter
 *
    Method assuming that loops in page graph are prohibited.
    We're going back to specific page if it is already on the stack.

 * \param page
 * \param properties
 */
void PageManager::enter(const Enums::Page page, QJsonObject properties, const bool immediate)
{
    qDebug() << CYAN("[PAGE] Print stack on enter") << m_pageStack;
    qDebug() << CYAN("[PAGE] Enter:") << page << "properties:" << properties;

    if (!m_popupStack.isEmpty()) {
        qWarning() << "Popup stack not empty:" <<  m_popupStack
                   << ". Returning as page cannot be pushed over popup!";
        return;
    }

    if (m_pageStack.contains(page)) {
        qWarning() << "Page" << page << "is already on the stack. Going back to page.";
        backTo(page);
        return;
    }

    m_pageStack.append(page);
    properties.insert(QStringLiteral("page"), static_cast<int>(page));
    emit stackViewPush(pageToQString(page), properties, immediate);
}

void PageManager::enterReplace(const Enums::Page page, QJsonObject properties, const bool immediate)
{
    qDebug() << CYAN("[PAGE] Print stack on enter") << m_pageStack;
    qDebug() << CYAN("[PAGE] Enter replace:") << page << "properties:" << properties;

    if (!m_popupStack.isEmpty()) {
        qWarning() << "Popup stack not empty:" <<  m_popupStack
                   << ". Returning as page cannot be pushed over popup!";
        return;
    }

    if (m_pageStack.contains(page)) {
        qWarning() << "Page" << page << "is already on the stack. Going back to page.";
        backTo(page);
        return;
    }

    m_pageStack.removeLast();
    m_pageStack.append(page);
    properties.insert(QStringLiteral("page"), static_cast<int>(page));
    emit stackViewReplace(pageToQString(page), properties, immediate);
}

void PageManager::openPopup(const Enums::Popup popup, QJsonObject properties)
{
    qDebug() << CYAN("[POPUP] Print stack on enter") << m_pageStack;
    qDebug() << CYAN("[POPUP] Enter:") << popup << "properties:" << properties;

    m_popupStack.append(popup);
    emit popupManagerOpen(pageToQString(popup), properties);
}

void PageManager::closePopup()
{
    qDebug() << CYAN("[PAGE] Print stack on pop") << m_pageStack;
    qDebug() << CYAN("[POPUP] Print stack on pop") << m_popupStack;

    if (m_popupStack.isEmpty()) {
        qWarning() << "Closing empty popup! Aborting!";
        return;
    }

    auto poppedPopup = m_popupStack.takeLast();
    qDebug() << "Closing popup" << pageToQString(poppedPopup);
    emit popupManagerClose();
}

void PageManager::sendAction(Enums::PopupAction action)
{
    // close popup first
    closePopup();

    emit popupAction(action);
}

void PageManager::back(const bool immediate)
{
    qDebug() << CYAN("[PAGE] Print stack on pop") << m_pageStack;
    qDebug() << CYAN("[POPUP] Print stack on pop") << m_popupStack;

    if (!m_popupStack.isEmpty()) {
        qWarning() << "Trying back with popup opened! Aborting!";
        return;
    }

    if (m_pageStack.isEmpty()) {
        qWarning() << "Popping empty stack! Aborting!";
        return;
    }

    // Prohibit from clearing stack by pop
    if (m_pageStack.count() == 1)
        return;

    auto poppedPage = m_pageStack.takeLast();
    qDebug() << "Popped page" << pageToQString(poppedPage);
    emit stackViewPop(immediate);
}

bool PageManager::backTo(const Enums::Page page, const bool immediate)
{
    if (!m_popupStack.isEmpty()) {
        qWarning() << "Trying back with popup opened! Aborting!";
        return false;
    }

    if (!m_pageStack.contains(page)) {
        qWarning() << "Page" << page << "is not on the stack. Returning.";
        return false;
    }

    Enums::Page currentTop = m_pageStack.last();
    while (currentTop != page) {
        m_pageStack.pop_back();
        currentTop = m_pageStack.last();
    }

    qDebug() << "Going back to page" << pageToQString(page);
    emit stackViewPopTo(page, immediate);
    return true;
}

QString PageManager::getInitialPageUrl() const
{
    return pageToQString(m_initialPage);
}

Enums::Page PageManager::homePage() const
{
    return m_homePage;
}

bool PageManager::isOnHomePage() const
{
    return isOnTop(m_homePage);
}

bool PageManager::isBackToHomePage() const
{
    if (m_pageStack.size() < 2) {
        return false;
    }

    return m_pageStack.at(m_pageStack.size() - 2) == m_homePage;
}

bool PageManager::isOnTop(Enums::Page page) const
{
    return m_pageStack.last() == page;
}
