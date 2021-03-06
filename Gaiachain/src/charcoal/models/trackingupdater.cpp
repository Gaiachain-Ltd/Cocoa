#include "trackingupdater.h"

#include "charcoal/database/charcoaldbhelpers.h"
#include "charcoal/picturesmanager.h"
#include "database/dbhelpers.h"
#include "common/logs.h"
#include "common/tags.h"

#include <QVariant>
#include <QFileInfo>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QSqlQuery>
#include <QSqlError>

TrackingUpdater::TrackingUpdater(const QString &connectionName)
    : m_connectionName(connectionName)
{
    if (isValid() == false) {
        qWarning() << RED("Invalid table name or connection name!")
                   << m_connectionName;
    }
}

void TrackingUpdater::setPicturesManager(PicturesManager *manager)
{
    m_picturesManager = manager;
}

UpdateResult TrackingUpdater::updateTable(const QJsonDocument &json) const
{
    const QJsonObject mainObject(json.object());
    const QJsonArray mainArray(mainObject.value(Tags::results).toArray());
    UpdateResult result;

    // We go back-to-front, in order to register oldest events first
    for (int i = mainArray.size() - 1; i >= 0; --i) {
        result = processTrackingItem(mainArray.at(i).toObject(), result);
        if (result.success == false) {
            return result;
        }
    }

    result.success = true;
    return result;
}

bool TrackingUpdater::updateDetails(const QJsonDocument &json) const
{
    const QJsonObject package(json.object());
    const int webId(package.value(Tags::id).toInt(-1));
    const QString pid(package.value(Tags::pid).toString());
    const QJsonObject properties(package.value(Tags::properties).toObject());

    qDebug() << "Updating Package number" << webId << pid;

    bool result = true;
    for (const QString &key : properties.keys()) {
        if (key == "logging_beginning") {
            if (processDetailsLoggingBeginning(properties.value(key).toObject()) == false) {
                result = false;
            }
        } else if (key == "logging_ending") {
            if (processDetailsLoggingEnding(properties.value(key).toObject()) == false) {
                result = false;
            }
        } else if (key == "ovens") {
            if (processDetailsOvens(webId, properties.value(key).toArray()) == false) {
                result = false;
            }
        } else if (key == "loading_transport") {
            if (processDetailsLoadingAndTransport(
                    pid, properties.value(key).toObject()) == false) {
                result = false;
            }
        } else if (key == "reception") {
            if (processDetailsReception(properties.value(key).toObject()) == false) {
                result = false;
            }
        }
    }

    return result;
}

bool TrackingUpdater::isValid() const
{
    return m_connectionName.isEmpty() == false;
}

UpdateResult TrackingUpdater::processTrackingItem(const QJsonObject &object,
                                                  UpdateResult result) const
{
    const int webId(object.value(Tags::id).toInt(-1));
    const int parentWebId(object.value(Tags::parentId).toInt(-1));
    const QString pid(object.value(Tags::pid).toString());
    const QString webType(object.value(Tags::type).toString());
    const Enums::PackageType type = packageType(webType);
    const int typeId(CharcoalDbHelpers::getEntityTypeId(m_connectionName, type));
    const bool isReplanted(object.value("plot_has_replantation").toBool(false));
    const bool isFinished(object.value("is_finished").toBool(false));

    // Not parsed and not needed: "type_display" property
    const QJsonArray events(object.value("entities").toArray());

    int entityId = CharcoalDbHelpers::getEntityIdFromWebId(
        m_connectionName, webId, db::QueryFlag::Silent);

    // Set proper parent
    const int parentEntityId = CharcoalDbHelpers::getEntityIdFromWebId(
        m_connectionName, parentWebId,
        db::QueryFlag::Silent | db::QueryFlag::MatchLast);

    /*
     * Web uses "parent" to mean direct parent. So Transport has parent Harvest.
     * Harvest has parent Plot.
     *
     * But Qt app works in a different way: parent is ALWAYS a Plot.
     *
     * Thus, we need to run parent search again when we process Transport
     * element - to get up to Plot level.
     */

    {
        QString entityString;
        if (entityId == -1) {
            // Entity does not exist in our DB - add it!
            entityString = "INSERT INTO Entities (typeId, name, parent, "
                           "webId, "
                           "isFinished, isReplanted) "
                           "VALUES (:typeId, :name, :parent, "
                           ":webId, "
                           ":isFinished, :isReplanted)";
        } else {
            QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
            query.prepare("SELECT isReplanted, isFinished FROM Entities "
                          "WHERE id=:entityId");
            query.bindValue(":entityId", entityId);

            if (query.exec() == false || query.next() == false) {
                qWarning() << RED("Checking existing entity has failed!")
                           << query.lastError().text()
                           << "for query:" << query.lastQuery();
                result.success = false;
                return result;
            }

            //const bool localReplanted = bool(query.value("isReplanted").toInt());
            const bool localFinished = bool(query.value("isFinished").toInt());

            //if (localReplanted && !isReplanted) {
            //    qDebug() << "R Local state:" << localReplanted
            //             << "remote:" << isReplanted;
            //    // Web has to be informed that package has been replanted!
            //    result.toReplant.append(webId);
            //}

            if (localFinished && !isFinished) {
                qDebug() << "F Local state:" << localFinished
                         << "remote:" << isFinished;
                // Web has to be informed that package has been finished!
                result.toFinish.append(webId);
            }

            // update Entity if it exists!
            entityString = "UPDATE Entities SET typeId=:typeId, name=:name, "
                           "parent=:parent, webId=:webId, "
                           "isFinished=:isFinished, isReplanted=:isReplanted "
                           "WHERE id=:entityId";
        }

        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
        query.prepare(entityString);
        query.bindValue(":typeId", typeId);
        query.bindValue(":name", pid);
        if (parentEntityId != -1) {
            query.bindValue(":parent", parentEntityId);
        } else {
            query.bindValue(":parent", QVariant(QVariant::Int));
        }
        query.bindValue(":webId", webId);
        query.bindValue(":isFinished", int(isFinished));
        query.bindValue(":isReplanted", int(isReplanted));
        if (entityId != -1) {
            query.bindValue(":entityId", entityId);
        }

        if (query.exec() == false) {
            qWarning() << RED("Inserting or updating Entity has failed!")
                       << query.lastError().text()
                       << "for query:" << query.lastQuery()
                       << entityId;
            result.success = false;
            return result;
        }

        //qDebug() << "TRACKING: ENTITY update!" << pid << isFinished << "!";

        if (entityId == -1) {
            entityId = query.lastInsertId().toInt();
        }
    }

    for (const QJsonValue &value : events) {
        const QJsonObject event(value.toObject());
        const int eventWebId(event.value(Tags::id).toInt(-1));
        const qint64 timestamp = event.value(Tags::timestamp).toVariant().toLongLong();
        const qint64 eventDate = event.value(Tags::webEventDate).toVariant().toLongLong();
        const QJsonArray location = event.value("location_display").toArray();
        const int eventTypeId = CharcoalDbHelpers::getEventTypeId(
            m_connectionName, event.value(Tags::action).toString());
        const QString userId(event.value("user_code").toString());

        int eventId = CharcoalDbHelpers::getEventIdFromWebId(
            m_connectionName, eventWebId, db::QueryFlag::Silent);

        if (eventId == -1) {
            eventId = CharcoalDbHelpers::getEventId(m_connectionName, entityId,
                                                    eventTypeId, timestamp, false);
        }

        QString eventString;
        if (eventId == -1) {
            eventString = "INSERT INTO Events (entityId, typeId, userId, "
                          "webId, parentWebId, "
                          "date, eventDate, locationLatitude, locationLongitude, "
                          "isCommitted) "
                          "VALUES (:entityId, :typeId, :userId, "
                          ":eventWebId, :parentWebId, "
                          ":date, :eventDate, :locationLatitude, :locationLongitude, "
                          "1)";
        } else {
            eventString = "UPDATE Events SET "
                          "entityId=:entityId, typeId=:typeId, userId=:userId, "
                          "webId=:eventWebId, parentWebId=:parentWebId, "
                          "date=:date, eventDate=:eventDate, "
                          "locationLatitude=:locationLatitude, "
                          "locationLongitude=:locationLongitude, "
                          "isCommitted=1 "
                          "WHERE id=:eventId";
        }

        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
        query.prepare(eventString);
        query.bindValue(":entityId", entityId);
        query.bindValue(":typeId", eventTypeId);
        query.bindValue(":userId", userId);
        query.bindValue(":eventWebId", eventWebId);
        query.bindValue(":parentWebId", webId);
        query.bindValue(":date", timestamp);
        query.bindValue(":eventDate", eventDate);
        query.bindValue(":locationLatitude", location.at(0).toDouble());
        query.bindValue(":locationLongitude", location.at(1).toDouble());

        if (eventId != -1) {
            query.bindValue(":eventId", eventId);
        }

        if (query.exec() == false) {
            qWarning() << RED("Inserting or updating event has failed!")
                       << query.lastError().text() << "for query:" << query.lastQuery();
            result.success = false;
            return result;
        }
    }

    result.success = true;
    return result;
}

bool TrackingUpdater::processDetailsLoggingBeginning(const QJsonObject &object) const
{
    const QJsonObject webEntity(object.value(Tags::entity).toObject());
    const qint64 timestamp(webEntity.value(Tags::timestamp).toVariant().toLongLong());
    const int webId = webEntity.value(Tags::id).toInt();
    const qint64 eventDate = object.value(Tags::webBeginningDate).toVariant().toLongLong();
    const int parcelId = object.value("parcel_id").toInt();
    const QString village(object.value(Tags::webVillage).toString());
    const QString treeSpecies(object.value(Tags::webTreeSpecies).toString());
    const int villageId = CharcoalDbHelpers::getVillageId(m_connectionName, village);
    const int treeSpeciesId = CharcoalDbHelpers::getTreeSpeciesId(
        m_connectionName, treeSpecies);

    return updateEventDetails(webId, timestamp,
                              {
                                  { Tags::webParcel, parcelId },
                                  { Tags::webVillage, villageId },
                                  { Tags::webTreeSpecies, treeSpeciesId },
                                  { Tags::webEventDate, eventDate }
                              });
}

bool TrackingUpdater::processDetailsLoggingEnding(const QJsonObject &object) const
{
    const QJsonObject webEntity(object.value(Tags::entity).toObject());
    const qint64 timestamp(webEntity.value(Tags::timestamp).toVariant().toLongLong());
    const int webId = webEntity.value(Tags::id).toInt();
    const qint64 eventDate = object.value(Tags::webEndingDate).toVariant().toLongLong();
    const int numberOfTrees = object.value(Tags::webNumberOfTrees).toInt();

    return updateEventDetails(webId, timestamp,
                              {
                                  { Tags::webNumberOfTrees, numberOfTrees },
                                  { Tags::webEventDate, eventDate }
                              });
}

bool TrackingUpdater::processDetailsOvens(const int webId,
                                          const QJsonArray &array) const
{
    bool result = true;
    for (const auto &value : array) {
        const QJsonObject object(value.toObject());
        const int ovenWebId(object.value(Tags::id).toInt());
        const QString ovenName(object.value(Tags::webOvenId).toString());
        const int harvestEntityId(CharcoalDbHelpers::getEntityIdFromWebId(
            m_connectionName, webId));
        const int plotEntityId(CharcoalDbHelpers::getParentEntityId(
            m_connectionName, harvestEntityId));

        if (const QJsonObject cb(object.value("carbonization_beginning").toObject());
            cb.isEmpty() == false)
        {
            const QJsonObject webEntity(cb.value(Tags::entity).toObject());
            const qint64 timestamp(webEntity.value(Tags::timestamp)
                                       .toVariant().toLongLong());
            const int webId = webEntity.value(Tags::id).toInt();

            const QString ovenTypeName(cb.value("oven_type_display").toString());
            const qint64 eventDate = cb.value(Tags::webBeginningDate)
                                         .toVariant().toLongLong();

            const QJsonObject ovenMeasurements(cb.value("oven_measurements")
                                                   .toObject());
            const double height = ovenMeasurements.value(Tags::webOvenHeight)
                                      .toDouble();
            const double height2 = ovenMeasurements.value(Tags::webOvenHeight2)
                                      .toDouble();
            const double width = ovenMeasurements.value(Tags::webOvenWidth)
                                     .toDouble();
            const double length = ovenMeasurements.value(Tags::webOvenLength)
                                      .toDouble();
            const double volume = ovenMeasurements.value(Tags::webOvenVolume)
                                      .toDouble();

            const int ovenTypeId = CharcoalDbHelpers::getOvenTypeIdFromName(
                m_connectionName, ovenTypeName);

            QVariantMap properties {
                { Tags::webOvenType, ovenTypeId },
                { Tags::webEventDate, eventDate },
                { Tags::webOvenId, ovenWebId }
            };

            const bool isDefault = ovenTypeId == CharcoalDbHelpers::metalOvenType;
            OvenDimensions defaultDims;
            if (isDefault) {
                defaultDims = CharcoalDbHelpers::defaultOvenDimensions(
                    m_connectionName, ovenTypeId);
            } else {
                properties.insert(Tags::webOvenHeight, height);
                properties.insert(Tags::webOvenLength, length);
                properties.insert(Tags::webOvenWidth, width);
                properties.insert(Tags::webOvenHeight2, height2);
                properties.insert(Tags::webOvenVolume, volume);
            }

            bool r = true;
            if (updateEventDetails(webId, timestamp, properties) == false) {
                r = false;
            }

            const int eventId = CharcoalDbHelpers::getEventIdFromWebId(
                m_connectionName, webId);

            QSqlQuery q(QString(), db::Helpers::databaseConnection(m_connectionName));
            q.prepare("INSERT INTO Ovens (type, plot, carbonizationBeginning, name, "
                      "oven_height, oven_height2, oven_width, oven_length) "
                      "VALUES (:type, :plot, :event, :name, "
                      ":height, :height2, :width, :length)");
            q.bindValue(":type", ovenTypeId);
            q.bindValue(":plot", plotEntityId);
            q.bindValue(":event", eventId);
            q.bindValue(":name", ovenName);

            if (isDefault) {
                q.bindValue(":height", defaultDims.height1);
                q.bindValue(":height2", defaultDims.height2);
                q.bindValue(":length", defaultDims.length);
                q.bindValue(":width", defaultDims.width);
            } else {
                q.bindValue(":height", height);
                q.bindValue(":height2", height2);
                q.bindValue(":length", length);
                q.bindValue(":width", width);
            }

            if (q.exec() == false) {
                qWarning() << RED("Inserting new oven has failed!")
                           << q.lastError().text() << "for query:" << q.lastQuery();
                r = false;
            }

            if (r == false) {
                result = false;
            }
        }

        if (const QJsonObject ce(object.value("carbonization_ending").toObject());
            ce.isEmpty() == false)
        {
            const QJsonObject webEntity(ce.value(Tags::entity).toObject());
            const qint64 timestamp(webEntity.value(Tags::timestamp)
                                       .toVariant().toLongLong());
            const int webId = webEntity.value(Tags::id).toInt();
            const qint64 eventDate = ce.value("end_date").toVariant().toLongLong();

            bool r = true;

            r = updateEventDetails(webId, timestamp,
                                   {
                                       { Tags::webOvenId, ovenName },
                                       { Tags::webEventDate, eventDate }
                                   });

            const int eventId = CharcoalDbHelpers::getEventIdFromWebId(
                m_connectionName, webId);
            const int ovenId = CharcoalDbHelpers::getOvenId(
                m_connectionName, plotEntityId, ovenName);

            QSqlQuery q(QString(), db::Helpers::databaseConnection(m_connectionName));
            q.prepare("UPDATE Ovens SET carbonizationEnding=:carbonizationEndingId "
                      "WHERE id=:ovenId");
            q.bindValue(":carbonizationEndingId", eventId);
            q.bindValue(":ovenId", ovenId);

            if (q.exec() == false) {
                qWarning() << RED("Updating oven has failed!")
                           << q.lastError().text() << "for query:" << q.lastQuery();
                r = false;
            }

            if (r == false) {
                result = false;
            }
        }
    }

    return result;
}

bool TrackingUpdater::processDetailsLoadingAndTransport(const QString &packageName,
                                                        const QJsonObject &object) const
{
    const QString harvestName(CharcoalDbHelpers::getHarvestName(packageName));
    const QJsonObject webEntity(object.value(Tags::entity).toObject());
    const qint64 timestamp(webEntity.value(Tags::timestamp).toVariant().toLongLong());
    const int webId = webEntity.value(Tags::id).toInt();
    const qint64 eventDate = object.value("loading_date").toVariant().toLongLong();
    const QString plateNumber = object.value(Tags::webPlateNumber).toString();
    const int destinationId = object.value("destination_id").toInt();
    const QStringList scannedQrs(getQrCodes(object.value(Tags::bags).toArray()));

    return updateEventDetails(webId, timestamp,
                              {
                                  { Tags::webPlateNumber, plateNumber },
                                  { Tags::webDestination, destinationId },
                                  { Tags::webQrCodes, scannedQrs },
                                  { Tags::webEventDate, eventDate }
                              });
}

bool TrackingUpdater::processDetailsReception(const QJsonObject &object) const
{
    const QJsonObject webEntity(object.value(Tags::entity).toObject());
    const qint64 timestamp(webEntity.value(Tags::timestamp).toVariant().toLongLong());
    const int webId = webEntity.value(Tags::id).toInt();
    const qint64 eventDate = object.value("reception_date").toVariant().toLongLong();

    const QStringList scannedQrs(getQrCodes(object.value(Tags::bags).toArray()));
    const QStringList docs(getImages(object.value(Tags::webDocuments).toArray()));
    const QStringList recs(getImages(object.value(Tags::webReceipts).toArray()));

    return updateEventDetails(webId, timestamp,
                              {
                                  { Tags::webDocuments, docs },
                                  { Tags::webReceipts, recs },
                                  { Tags::webQrCodes, scannedQrs },
                                  { Tags::webEventDate, eventDate }
                              });
}

bool TrackingUpdater::updateEventDetails(const int webId,
                                         const qint64 timestamp,
                                         const QVariantMap &properties) const
{
    Q_UNUSED(timestamp)

    const QString propString(CharcoalDbHelpers::propertiesToString(properties));
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    query.prepare("UPDATE Events SET properties=:properties "
                  "WHERE webId=:webId");
    query.bindValue(":properties", propString);
    query.bindValue(":webId", webId);

    if (query.exec() == false) {
        qDebug() << RED("Updating details has failed")
                 << query.lastError().text()
                 << "for query:" << query.lastQuery();
        return false;
    }

    // Some events are missing their webId, we update it here.
    const int updatedRows = query.numRowsAffected();
    if (updatedRows <= 0) {
        query.prepare("UPDATE Events SET properties=:properties, webId=:webId "
                      "WHERE date=:timestamp");
        query.bindValue(":properties", propString);
        query.bindValue(":webId", webId);
        query.bindValue(":timestamp", timestamp);

        if (query.exec() == false) {
            qDebug() << RED("Updating details has failed")
                     << query.lastError().text()
                     << "for query:" << query.lastQuery();
            return false;
        }
    }

    return true;
}

QStringList TrackingUpdater::getQrCodes(const QJsonArray &codes) const
{
    QStringList result;

    for (const auto &value : codes) {
        const QJsonObject bag(value.toObject());
        result.append(bag.value("qr_code").toString());
    }

    return result;
}

QStringList TrackingUpdater::getImages(const QJsonArray &images) const
{
    QStringList result;

    for (const auto &value : images) {
        const QJsonObject img(value.toObject());
        const QString path(img.value("image").toString());
        const QString fileName(QFileInfo(path).fileName());

        if (m_picturesManager) {
            m_picturesManager->checkFileIsCached(path);
        }

        result.append(fileName);
    }

    return result;
}

Enums::PackageType TrackingUpdater::packageType(const QString &webType) const
{
    if (webType == QLatin1String("TRK")) {
        return Enums::PackageType::Transport;
    } else if (webType == QLatin1String("HAR")) {
        return Enums::PackageType::Harvest;
    } else if (webType == QLatin1String("PLT")) {
        return Enums::PackageType::Plot;
    }

    return Enums::PackageType::Unknown;
}
