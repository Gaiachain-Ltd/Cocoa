﻿#include "commoditydaterangeproxymodel.h"

#include "commodityproxymodel.h"
#include "eventmodel.h"
#include "shipmentmodel.h"

#include <QTimer>

#include <QDebug>

CommodityDateRangeProxyModel::CommodityDateRangeProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
}

void CommodityDateRangeProxyModel::setCommodityProxyModel(CommodityProxyModel *commodityProxyModel)
{
    m_commodityProxyModel = commodityProxyModel;
    connect(m_commodityProxyModel, &CommodityProxyModel::filteringFinished, this, &CommodityDateRangeProxyModel::invalidateFilterNotify);
}

void CommodityDateRangeProxyModel::setDateTimeRange(QDateTime start, QDateTime end)
{
    m_startDateTime = start;
    m_endDateTime = end;

    qDebug() << "Start end dates" << start << end;
    m_filteredDates.clear();
    invalidateFilterNotify();
}

bool CommodityDateRangeProxyModel::isEventToday(QDate date)
{
    return m_filteredDates.contains(date);
}

bool CommodityDateRangeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto timestamp = sourceModel()->data(index, EventModel::Timestamp).toDateTime();

    bool containsId = false;
    if (isInDateTimeRange(timestamp)) {
        auto shipmentId = sourceModel()->data(index, EventModel::ShipmentId).toString();
        containsId = m_commodityProxyModel->hasShipmentId(shipmentId);
    }

    if (containsId)
        m_filteredDates.insert(timestamp.date());

    return containsId;
}

bool CommodityDateRangeProxyModel::isInDateTimeRange(QDateTime &dt) const
{
    return m_startDateTime <= dt && dt < m_endDateTime;
}

