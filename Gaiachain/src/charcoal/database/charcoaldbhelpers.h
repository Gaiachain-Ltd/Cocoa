#pragma once

#include "common/enums.h"

class QString;

class CharcoalDbHelpers
{
public:
    // TODO: unify these methods, do not do such massive copy-paste
    // TODO: transfer more generic methods away from ActionController and other
    // model classes
    static int getWebPackageId(const QString &connectionName, const int entityId);
    static int getVillageId(const QString &connectionName, const QString &name);
    static int getDestinationId(const QString &connectionName, const QString &name);
    static int getParcelId(const QString &connectionName, const QString &parcel);

    static int getEntityIdFromWebId(const QString &connectionName, const int webId,
                                    const bool verbose = true);
    static int getEntityIdFromName(const QString &connectionName, const QString &name);

    static int getEntityTypeId(const QString &connectionName, const Enums::PackageType type);

    static int getSimpleInteger(const QString &connectionName, const QString &table,
                                const QString &matchColumn, const QString &matchValue,
                                const QString &returnColumn, const bool verbose = true);

    static QString getOvenLetter(const QString &connectionName, const int ovenId);
    static QString getEventType(const QString &connectionName, const int typeId);
    static QString getEntityName(const QString &connectionName, const int entityId);

    static QString getSimpleString(const QString &connectionName, const QString &table,
                                   const QString &matchColumn, const QString &matchValue,
                                   const QString &returnColumn, const bool verbose = true);
};

