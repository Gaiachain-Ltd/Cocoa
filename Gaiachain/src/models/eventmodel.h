#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include "abstractmodel.h"

#include <QHash>

class EventModel : public AbstractModel
{
    Q_OBJECT

public:
    enum ModelRole {
        PackageId = Qt::UserRole,
        UserRole,
        CooperativeId,
        Timestamp,
        Action,
        Properties,
        LastRole
    }; //!!! Add new roles at the end

    explicit EventModel(QObject *parent = nullptr);

    int lastRole() const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    static const QHash<int, QByteArray> sc_roleNames;
};

#endif // EVENTMODEL_H
