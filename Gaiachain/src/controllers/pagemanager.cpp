#include "pagemanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "../helpers/utility.h"

PageManager::PageManager(QObject *parent) : AbstractManager(parent)
{
    prepareConnections();

    m_pageStack.push_back(m_initialPage);
    m_pageSectionsModel.stackReset(m_homePage);
}

void PageManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("pageManager"), this);
    engine.rootContext()->setContextProperty(QStringLiteral("sectionsModel"), &m_pageSectionsModel);
}

/*!
 * \brief PageManager::enterPage
 *
    Method assuming that loops in page graph are prohibited.
    We're going back to specific page if it is already on the stack.

 * \param page
 * \param properites
 */
void PageManager::enterPage(const Enums::Page page, QJsonObject properites, const bool immediate)
{
    qDebug() << "Print stack on enter" << m_pageStack;
    qDebug() << "Page properties:" << properites;

    if (m_pageStack.contains(page)) {
        qWarning() << "Page" << page << "is already on the stack. Going back to page.";
        backToPage(page);
        return;
    }

    if (page == Enums::Page::QRScanner)
        backToPage(m_homePage);

    bool isEditSection = false;
    if (page == Enums::Page::EventDetails)
        isEditSection = !properites.value("readOnly").toBool();

    m_pageStack.push_back(page);
    m_pageSectionsModel.pagePushed(page, isEditSection);
    properites.insert(QStringLiteral("page"), static_cast<int>(page));

    // TO_DO Add checking of page url correctness
    QString pageUrl = pageToQString(page);
    qDebug() << "Entered page" << pageUrl;

    emit stackViewPush(pageToQString(page), properites, immediate);
}

void PageManager::enterPopup(const Enums::Page page, QJsonObject properites)
{
    if (!properites.contains("isPopup"))
        properites.insert(QStringLiteral("isPopup"), true);

    enterPage(page, properites, true);
}

void PageManager::popPage(const bool immediate)
{
    qDebug() << "Print stack on pop" << m_pageStack;

    if (m_pageStack.isEmpty()) {
        qWarning() << "Popping empty stack! Aborting!";
        return;
    }

    // Prohibit from clearing stack by pop
    if (m_pageStack.count() == 1)
        return;

    auto poppedPage = m_pageStack.takeLast();
    m_pageSectionsModel.pagePopped(m_pageStack.last());

    qDebug() << "Popped page" << pageToQString(poppedPage);

    emit stackViewPop(immediate);
}

bool PageManager::backToPage(const Enums::Page backPage, const bool immediate)
{
    if (!m_pageStack.contains(backPage)) {
        qWarning() << "Page" << backPage << "is not on the stack. Returning.";
        return false;
    }

    Enums::Page currentTop = m_pageStack.last();
    while (currentTop != backPage) {
        m_pageStack.pop_back();
        currentTop = m_pageStack.last();
        m_pageSectionsModel.pagePopped(currentTop);
    }

    qDebug() << "Going back to page" << pageToQString(backPage);

    emit stackViewBackToPage(backPage, immediate);

    return true;
}

void PageManager::prepareConnections()
{
    connect(this, &PageManager::push, this, &PageManager::enterPage, Qt::DirectConnection);
    connect(this, &PageManager::pop, this, &PageManager::popPage, Qt::DirectConnection);
    connect(this, &PageManager::back, this, &PageManager::pop, Qt::DirectConnection);

    connect(this, &PageManager::backTo, this, &PageManager::backToPage, Qt::DirectConnection);
    connect(this, &PageManager::backToSection, this, &PageManager::backToFirstSectionPage, Qt::DirectConnection);
}

bool PageManager::backToFirstSectionPage(const Enums::PageSections section)
{
    Enums::Page page = m_pageSectionsModel.getPageForSection(section);
    return backToPage(page);
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
    return m_pageStack.last() == m_homePage;
}

QString PageManager::pageToQString(const Enums::Page p) const
{
    const QString pageStr = Utility::enumToQString<Enums::Page>(p, "Page");
    const QString pathPrefix = m_pagePrefix + pageStr;
    return pageStr.contains(QStringLiteral("Popup"))
           ? pathPrefix + QStringLiteral(".qml")
           : pathPrefix + QStringLiteral("Page.qml");
}
