#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>

#include "datamanager.h"
#include "pagemanager.h"

class QQmlApplicationEngine;

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = nullptr);

    void setupQMLContext(QQmlApplicationEngine &engine);

signals:

public slots:

private:
    DataManager m_dataManager;
    PageManager m_pageManager;
};

#endif // MAINCONTROLLER_H