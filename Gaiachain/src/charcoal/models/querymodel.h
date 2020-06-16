#pragma once

#include <QSqlQueryModel>

class QueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit QueryModel(QObject *parent = nullptr);

    void setDbQuery(const QString &query);
    void setDbConnection(const QString &connectionName);

    Q_INVOKABLE virtual void refresh();

protected:
    QString m_query;
    QString m_connectionName;
};

