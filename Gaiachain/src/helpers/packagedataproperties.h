#ifndef PACKAGEDATAPROPERTIES_H
#define PACKAGEDATAPROPERTIES_H

#include <QObject>

class PackageDataProperties : public QObject {
    Q_OBJECT
private:
    // property names
    Q_PROPERTY(QString ParcelId MEMBER ParcelId CONSTANT)
    Q_PROPERTY(QString BreakingDate MEMBER BreakingDate CONSTANT)
    Q_PROPERTY(QString FermentationEndDate MEMBER FermentationEndDate CONSTANT)
    Q_PROPERTY(QString EstimatedBeansVolume MEMBER EstimatedBeansVolume CONSTANT)
    Q_PROPERTY(QString DepartureDate MEMBER DepartureDate CONSTANT)
    Q_PROPERTY(QString HarvestDate MEMBER HarvestDate CONSTANT)
    Q_PROPERTY(QString DeparturePlace MEMBER DeparturePlace CONSTANT)
    Q_PROPERTY(QString Buyer MEMBER Buyer CONSTANT)
    Q_PROPERTY(QString Weights MEMBER Weights CONSTANT)
    Q_PROPERTY(QString Transporter MEMBER Transporter CONSTANT)
    Q_PROPERTY(QString Destination MEMBER Destination CONSTANT)
    Q_PROPERTY(QString Weight MEMBER Weight CONSTANT)
    Q_PROPERTY(QString ReceptionDate MEMBER ReceptionDate CONSTANT)
    Q_PROPERTY(QString TransportDate MEMBER TransportDate CONSTANT)

public:
    static PackageDataProperties &instance();

    // property names
    static const QString ParcelId;
    static const QString BreakingDate;
    static const QString FermentationEndDate;
    static const QString EstimatedBeansVolume;
    static const QString DepartureDate;
    static const QString HarvestDate;
    static const QString DeparturePlace;
    static const QString Buyer;
    static const QString Weights;
    static const QString Transporter;
    static const QString Destination;
    static const QString Weight;
    static const QString ReceptionDate;
    static const QString TransportDate;

private:
    PackageDataProperties(QObject *parent=nullptr);


};

#endif // PACKAGEDATAPROPERTIES_H
