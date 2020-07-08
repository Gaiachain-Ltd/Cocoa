#include "unusedtransportidsmodel.h"

UnusedTransportIdsModel::UnusedTransportIdsModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT name FROM Entities WHERE isFinished=0 AND typeId IN "
               "(SELECT id FROM EntityTypes WHERE name=\"Transport\")");
}