#pragma once

#include "querymodel.h"
#include "common/enums.h"

class TrackingModel : public QueryModel
{
    Q_OBJECT

public:
    enum TrackingRole {
        Id = Qt::ItemDataRole::UserRole + 1,
        Name,
        Type,
        Events,
        EventSummary
    };
    Q_ENUM(TrackingRole)

    explicit TrackingModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void refreshWebData() override;

protected slots:
    void webReplyHandler(const QJsonDocument &reply) override;

private:
    QHash<int, QByteArray> m_roleNames = {
        { TrackingRole::Id, "eventId" },
        { TrackingRole::Name, "name" },
        { TrackingRole::Type, "type" },
        { TrackingRole::Events, "events" },
        { TrackingRole::EventSummary, "eventSummary" }
    };
};

