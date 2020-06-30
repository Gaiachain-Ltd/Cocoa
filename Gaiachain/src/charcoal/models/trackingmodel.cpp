#include "trackingmodel.h"

#include "database/dbhelpers.h"
#include "rest/baserequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "common/tags.h"
#include "helpers/utility.h"
#include "trackingupdater.h"
#include "charcoal/database/charcoaldbhelpers.h"
#include "charcoal/picturesmanager.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDate>

TrackingModel::TrackingModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, typeId, name, parent, webId FROM Entities");
}

void TrackingModel::setPicturesManager(PicturesManager *manager)
{
    m_picturesManager = manager;
}

QVariant TrackingModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() == false) {
        return {};
    }

    query().seek(index.row());

    switch (role) {
    case TrackingRole::Id:
        return query().value("id").toString();
    case TrackingRole::Name:
    case Qt::ItemDataRole::DisplayRole:
        return query().value("name").toString();
    case TrackingRole::Type:
    {
        const int id(query().value("typeId").toInt());
        return int(CharcoalDbHelpers::getEntityType(m_connectionName, id));
    }
    case TrackingRole::Events:
    {
        Entity entity;
        entity.id = query().value("id").toInt();
        entity.parent = query().value("parent").toInt();
        entity.typeId = query().value("typeId").toInt();
        entity.name = query().value("name").toString();
        const QVector<Event> events = entity.loadEvents(m_connectionName);

        QVariantList result;

        for (const Event &event : events) {
            const auto action = CharcoalDbHelpers::actionById(m_connectionName,
                                                              event.typeId);
            QString name;
            switch (action) {
            case Enums::SupplyChainAction::LoggingBeginning:
                name = tr("Logging has begun");
                break;
            case Enums::SupplyChainAction::LoggingEnding:
                name = tr("Logging has ended");
                break;
            case Enums::SupplyChainAction::CarbonizationBeginning:
            {
                const Oven oven = event.loadOven(m_connectionName);
                name = tr("Oven %1 - carbonization has begun").arg(oven.name);
            }
            break;
            case Enums::SupplyChainAction::CarbonizationEnding:
            {
                const Oven oven = event.loadOven(m_connectionName);
                name = tr("Oven %1 - carbonization has ended").arg(oven.name);
            }
            break;
            case Enums::SupplyChainAction::LoadingAndTransport:
            {
                name = tr("Bags have been loaded on truck %1")
                           .arg(event.properties.value(Tags::webPlateNumber).toString());
            }
            break;
            case Enums::SupplyChainAction::Reception:
                name = tr("Reception at storage facility");
                break;
            default: name = QString();
            }

            const QVariantMap row {
                { "eventName", name },
                { "date", event.date }
            };

            result.append(row);
        }

        return result;
    }
    case TrackingRole::EventSummary:
    {
        /*
         * This scary code looks heavy (and it is) - but it is called only
         * when user clicks on a delegate on Tracking page.
         */

        Entity entity;
        entity.id = query().value("id").toInt();
        entity.parent = query().value("parent").toInt();
        entity.typeId = query().value("typeId").toInt();
        entity.name = query().value("name").toString();

        const Enums::PackageType entityType = CharcoalDbHelpers::getEntityType(
            m_connectionName, entity.typeId);

        const QVector<Event> events = entity.loadEvents(m_connectionName);

        QVariantList result;
        switch (entityType) {
        case Enums::PackageType::Plot:
            result = summaryForPlot(entity, events);
            break;
        case Enums::PackageType::Harvest:
            result = summaryForHarvest(entity, events);
            break;
        case Enums::PackageType::Transport:
            result = summaryForTransport(entity, events);
            break;
        default:
            result.append(Utility::instance().createSummaryItem(
                "default", "error"));
        }
        return result;
    }
    }

    return {};
}

QHash<int, QByteArray> TrackingModel::roleNames() const
{
    return m_roleNames;
}

QVariantList TrackingModel::summaryForPlot(
    const Entity entity,
    const QVector<Event> &events) const
{
    const auto &utility = Utility::instance();

    QVariantList result;

    int parcelId = -1;
    int villageId = -1;
    int treeSpeciesId = -1;
    qint64 beginningTimestamp = -1;
    qint64 endingTimestamp = -1;

    if (events.isEmpty() == false) {
        const auto &first = events.at(0).properties;
        parcelId = first.value(Tags::webParcel).toInt(-1);
        villageId = first.value(Tags::webVillage).toInt(-1);
        treeSpeciesId = first.value(Tags::webTreeSpecies).toInt(-1);
        beginningTimestamp = first.value(Tags::webEventDate).toVariant().toLongLong();

        if (events.length() > 1) {
            endingTimestamp = events.at(1).properties.value(Tags::webEventDate)
                                  .toVariant().toLongLong();
        }
    }

    QString parcel;
    if (parcelId != -1) {
        parcel = CharcoalDbHelpers::getParcelCode(m_connectionName, parcelId);
    }

    QString village;
    if (villageId != -1) {
        village = CharcoalDbHelpers::getVillageName(m_connectionName, villageId);
    }

    QString treeSpecies;
    if (treeSpeciesId != -1) {
        treeSpecies = CharcoalDbHelpers::getTreeSpeciesName(m_connectionName, treeSpeciesId);
    }

    result.append(utility.createSummaryItem(
        tr("Plot ID"), entity.name, QString(), QString(),
        m_plotHighlightColor, m_plotTextColor, QString(),
        Enums::DelegateType::Standard));
    if (events.isEmpty() == false) {
        result.append(utility.createSummaryItem(
            tr("Malebi Rep's ID"), events.at(0).userId));
    }
    result.append(utility.createSummaryItem(
        tr("Parcel"), parcel));
    result.append(utility.createSummaryItem(
        tr("Village"), village));
    result.append(utility.createSummaryItem(
        tr("Beginning date"), dateString(beginningTimestamp)));
    result.append(utility.createSummaryItem(
        tr("Ending date"), dateString(endingTimestamp)));
    result.append(utility.createSummaryItem(
        tr("Tree species"), treeSpecies));
    return result;
}

QVariantList TrackingModel::summaryForHarvest(
    const TrackingModel::Entity entity,
    const QVector<TrackingModel::Event> &events) const
{
    const auto &utility = Utility::instance();

    QVariantList result;

    const QString plotName(CharcoalDbHelpers::getPlotName(entity.name));

    Utility::SummaryValue plotValue;
    {
        plotValue.titles.append(tr("Plot ID"));
        plotValue.values.append(plotName);

        Entity plotEntity;
        if (plotEntity.loadFromDb(m_connectionName, entity.parent)) {
            const QVector<Event> plotEvents(plotEntity.loadEvents(m_connectionName));
            plotValue.icons.append("qrc:/ui/loupe");
            plotValue.linkDestinationPages.append(Enums::Page::SupplyChainSummary);
            plotValue.linkDatas.append(QVariantMap{
                {"title", tr("Plot ID details")},
                {"proceedButtonVisible", false},
                {"summary", summaryForPlot(plotEntity, plotEvents)}
            });
        }
    }

    result.append(utility.createSummaryItem(
        tr("Harvest ID"), entity.name, QString(), QString(),
        m_harvestHighlightColor, m_harvestTextColor, QString(),
        Enums::DelegateType::Standard));
    result.append(utility.createSummaryItem(
        QString(),
        plotValue.toList(),
        QString(), QString(),
        m_plotHighlightColor, m_plotTextColor, QString(),
        Enums::DelegateType::ColumnStack));

    // TODO: collapsible oven summary!

    // QMap because we want it to be *sorted*
    QMap<QString, Oven> ovens;
    for (const auto &event : events) {
        const Oven oven = event.loadOven(m_connectionName);
        if (ovens.contains(oven.name)) {
            Oven existing = ovens.value(oven.name);
            existing.updateDates(oven, event);
            ovens.insert(oven.name, existing);
        } else {
            ovens.insert(oven.name, oven);
        }
    }

    for (const auto &ovenName : ovens.keys()) {
        const Oven oven = ovens.value(ovenName);
        result.append(utility.createSummaryItem(
            tr("Oven %1").arg(oven.name), QString()
            ));
        result.append(utility.createSummaryItem(
            tr("Beginning date"), dateString(oven.carbonizationBeginning)
            ));
        result.append(utility.createSummaryItem(
            tr("Ending date"), dateString(oven.carbonizationEnding)
            ));
        result.append(utility.createSummaryItem(
            tr("Carbonizer ID"), oven.carbonizerId));
        result.append(utility.createSummaryItem(
            tr("Oven measurement (meters)"),
            QVariantList {
                QVariantList {
                    tr("Height"),
                    tr("Length"),
                    tr("Width")
                },
                QVariantList {
                    oven.height,
                    oven.length,
                    oven.width
                }
            },
            QString(), QString(), QString(), QString(), QString(),
            Enums::DelegateType::Row));
        result.append(utility.createSummaryItem(
            tr("Timber volume"), QString("%1 m³").arg(oven.volume())
            ));
    }

    return result;
}

QVariantList TrackingModel::summaryForTransport(
    const TrackingModel::Entity entity,
    const QVector<TrackingModel::Event> &events) const
{
    const auto &utility = Utility::instance();

    QVariantList result;

    qint64 beginningTimestamp = -1;
    qint64 endingTimestamp = -1;
    QString plateNumber;
    QVariantList receptionData;

    if (events.isEmpty() == false) {
        const auto &first = events.at(0).properties;
        plateNumber = first.value(Tags::webPlateNumber).toString();
        beginningTimestamp = first.value(Tags::webEventDate).toVariant().toLongLong();

        if (events.length() > 1) {
            const auto &second = events.at(1).properties;
            endingTimestamp = second.value(Tags::webEventDate)
                                  .toVariant().toLongLong();

            QStringList docs;
            QStringList recs;

            const QString cache(m_picturesManager->cachePath());

            const QJsonArray docsArray(second.value(Tags::documents).toArray());
            for (const auto &value : docsArray) {
                docs.append(cache + "/" + value.toString());
            }

            const QJsonArray recsArray(second.value(Tags::receipts).toArray());
            for (const auto &value : recsArray) {
                recs.append(cache + "/" + value.toString());
            }

            const bool hasDocs = (docs.isEmpty() == false);
            const bool hasRecs = (recs.isEmpty() == false);

            m_picturesManager->setCurrentDocuments(docs);
            m_picturesManager->setCurrentReceipts(recs);

            const QString uploaded(tr("Uploaded"));
            const QString noPhoto(tr("No photo"));

            const QString bagNumberString(QString::number(
                CharcoalDbHelpers::bagCountInTransport(m_connectionName, entity.id)));

            Utility::SummaryValue value;
            value.titles = QStringList {
                bagNumberString,
                dateString(beginningTimestamp),
                QString(hasDocs? uploaded : noPhoto),
                QString(hasRecs? uploaded : noPhoto)
            };

            value.values = QStringList {
                tr("Number of bags"),
                tr("Loading date"),
                tr("Documents"),
                tr("Receipts")
            };

            value.icons = QStringList {
                QString(),
                QString(),
                "image://tickmark/document-" + QString(hasDocs? "true" : "false"),
                "image://tickmark/receipt-" + QString(hasRecs? "true" : "false")
            };

            value.linkDestinationPages = QList<Enums::Page>{
                Enums::Page::InvalidPage,
                Enums::Page::InvalidPage,
                (hasDocs? Enums::Page::PhotoGallery : Enums::Page::InvalidPage),
                (hasRecs? Enums::Page::PhotoGallery : Enums::Page::InvalidPage)
            };

            value.linkDatas = QVariantList {
                QVariant(),
                QVariant(),
                QVariantMap{ {"urls", docs} },
                QVariantMap{ {"urls", recs} }
            };

            receptionData = value.toList();
        }
    }

    result.append(utility.createSummaryItem(
        tr("Transport ID"), entity.name, QString(), QString(),
        m_transportHighlightColor, m_transportTextColor, QString(),
        Enums::DelegateType::Standard));
    result.append(utility.createSummaryItem(
        QString(), receptionData,
        QString(), QString(),
        m_harvestHighlightColor, m_harvestTextColor, QColor("000000"),
        Enums::DelegateType::ColumnStack));
    result.append(utility.createSummaryItem(
        tr("Plate number"), plateNumber));
    result.append(utility.createSummaryItem(
        tr("Reception at the storage facility"), dateString(endingTimestamp)));

    return result;
}

void TrackingModel::refreshWebData()
{
    // TODO: limit amount of data with canFetchMore() etc.
    // TODO: populate more data periodically (package details!)
    const auto request = QSharedPointer<BaseRequest>::create(
        "/entities/packages/",
        BaseRequest::Type::Get);

    if (m_userManager->isLoggedIn()) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &TrackingModel::webErrorHandler,
                                      &TrackingModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequests.append(request);
    }
}

void TrackingModel::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Data is:" << reply;
    TrackingUpdater updater(m_connectionName);
    if (updater.updateTable(reply)) {
        emit webDataRefreshed();
    } else {
        emit error(tr("Error updating tracking information"));
    }
}

QString TrackingModel::dateString(const qint64 timestamp) const
{
    return QDateTime::fromSecsSinceEpoch(timestamp).toString(QStringLiteral("dd/MM/yyyy"));
}

bool TrackingModel::Entity::loadFromDb(const QString &connectionName, const int id)
{
    QSqlQuery q(QString(), db::Helpers::databaseConnection(connectionName));
    q.prepare("SELECT id, typeId, name, parent, webId FROM Entities "
                  "WHERE id=:id");
    q.bindValue(":id", id);

    if (q.exec() && q.next()) {
        this->id = q.value("id").toInt();
        parent = q.value("parent").toInt();
        typeId = q.value("typeId").toInt();
        name = q.value("name").toString();
        return true;
    } else {
        qWarning() << "Could not load Entity from DB"
                   << "SQL error:" << q.lastError()
                   << "For query:" << q.lastQuery()
                   << id;
    }

    return false;
}

QVector<TrackingModel::Event> TrackingModel::Entity::loadEvents(
    const QString &connectionName) const
{
    QSqlQuery q(QString(), db::Helpers::databaseConnection(connectionName));
    q.prepare("SELECT id, typeId, userId, eventDate, date, properties FROM Events "
              "WHERE entityId=:id");
    q.bindValue(":id", id);

    if (q.exec() == false) {
        qWarning() << RED("Getting event details has failed!")
                   << q.lastError().text() << "for query:" << q.lastQuery();
        return {};
    }

    QVector<Event> events;
    while (q.next()) {
        Event event;
        event.id = q.value("id").toInt();
        event.typeId = q.value("typeId").toInt();
        event.userId = q.value("userId").toString();
        event.date = q.value("eventDate").toLongLong();
        event.timestamp = q.value("date").toLongLong();
        event.properties = QJsonDocument::fromJson(
                               q.value("properties").toString().toLatin1())
                               .object();
        events.append(event);
    }

    return events;
}

TrackingModel::Oven TrackingModel::Event::loadOven(const QString &connectionName) const
{
    QSqlQuery q(QString(), db::Helpers::databaseConnection(connectionName));
    q.prepare("SELECT id, type, plot, "
              "carbonizationBeginning, carbonizationEnding, "
              "name, "
              "oven_height, oven_width, oven_length "
              "FROM Ovens "
              "WHERE carbonizationBeginning=:id OR carbonizationEnding=:id");
    q.bindValue(":id", id);

    if (q.exec() == false || q.next() == false) {
        qWarning() << RED("Getting oven details has failed!")
                   << q.lastError().text() << "for query:" << q.lastQuery() << id;
        return {};
    }

    Oven oven;
    oven.id = q.value("id").toInt();
    oven.typeId = q.value("type").toInt();
    oven.plotId = q.value("plot").toInt();
    oven.name = q.value("name").toString();
    oven.height = q.value("oven_height").toReal();
    oven.width = q.value("oven_width").toReal();
    oven.length = q.value("oven_length").toReal();
    oven.carbonizerId = userId;

    const QVariant beginningEvent(q.value("carbonizationBeginning"));
    if (beginningEvent.isValid() && beginningEvent.toInt() == id) {
        oven.carbonizationBeginningId = id;
        oven.carbonizationBeginning = date;
    }

    const QVariant endingEvent(q.value("carbonizationEnding"));
    if (endingEvent.isValid() && endingEvent.toInt() == id) {
        oven.carbonizationEndingId = id;
        oven.carbonizationEnding = date;
    }

    return oven;
}

void TrackingModel::Oven::updateDates(const TrackingModel::Oven &other,
                                      const TrackingModel::Event &otherEvent)
{
    if (other.carbonizationBeginningId != -1) {
        carbonizationBeginningId = other.carbonizationBeginningId;
        carbonizationBeginning = otherEvent.date;
    }

    if (other.carbonizationEndingId != -1) {
        carbonizationEndingId = other.carbonizationEndingId;
        carbonizationEnding = otherEvent.date;
    }
}
