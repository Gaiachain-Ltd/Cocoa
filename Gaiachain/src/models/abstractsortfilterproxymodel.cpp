#include "abstractsortfilterproxymodel.h"

AbstractSortFilterProxyModel::AbstractSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    ModelChangedExtension::setupConnections(this);

    setDynamicSortFilter(true);
}
