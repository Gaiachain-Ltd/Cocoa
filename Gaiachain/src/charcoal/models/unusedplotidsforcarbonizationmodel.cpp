#include "unusedplotidsforcarbonizationmodel.h"

UnusedPlotIdsForCarbonizationModel::UnusedPlotIdsForCarbonizationModel(QObject *parent)
    : SimpleListQueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, name FROM Entities WHERE isFinished=0 "
               "AND typeId IN (SELECT id FROM EntityTypes WHERE name=\"Plot\") "
               "AND SELECT (COUNT(DISTINCT id) = 2) FROM Events "
               "WHERE entityId=Entities.id)");
    // Query does ONLY show Entities where 2 events are already defined (both
    // logging beginning and logging ending). Only such plots can be carbonized
}