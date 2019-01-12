#ifndef COMMODITYDATERANGEPROXYMODEL_H
#define COMMODITYDATERANGEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDateTime>
#include <QDate>
#include <QSet>

#include "abstractsortfilterproxymodel.h"
#include "../common/enums.h"

class CommodityProxyModel;

class CommodityDateRangeProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CommodityDateRangeProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setDateTimeRange(QDateTime start, QDateTime end);
    Q_INVOKABLE bool isEventToday(QDate date);

    void setCommodityProxyModel(CommodityProxyModel *commodityProxyModel);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;

private:
    CommodityProxyModel *m_commodityProxyModel = nullptr;
    mutable QSet<QDate> m_filteredDates;

    QDateTime m_startDateTime;
    QDateTime m_endDateTime;

    inline bool isInDateTimeRange(QDateTime &dt) const;
};

#endif // COMMODITYDATERANGEPROXYMODEL_H
