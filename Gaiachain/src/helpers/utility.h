#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>
#include <QString>
#include <QMetaEnum>
#include <QDate>

#include "../common/enums.h"

class Utility : public QObject
{
    Q_OBJECT
public:
    static Utility *instance();

    template<typename QEnum>
    static QString enumToQString(QEnum value, const char *enumName)
    {
        const QMetaObject smo = Enums::staticMetaObject;
        const int indexOfType = smo.indexOfEnumerator(enumName);
        const QMetaEnum enumerator = smo.enumerator(indexOfType);
        return QString(enumerator.valueToKey(static_cast<int>(value)));
    }

    Q_INVOKABLE QString commodityToString(Enums::CommodityType ct);

    Q_INVOKABLE qreal scaleByDpi(qreal num) const;
    Q_INVOKABLE qreal scaleRoundByDpi(qreal num) const;

    Q_INVOKABLE qreal clamp(qreal v, qreal min, qreal max) const;

    Q_INVOKABLE int parseInt(const QString &num) const;
    Q_INVOKABLE QDate convertDateString(const QString &dateStr, const QString &dateFormat = QString()) const;
    Q_INVOKABLE QString defaultDateFormat() const;

    Q_INVOKABLE bool validateId(const QString &id) const;
    Q_INVOKABLE bool validateEmail(const QString &email) const;

    Q_INVOKABLE int getScannedIdLength() const;

    Q_INVOKABLE bool isWeekend(const QDate &date) const;

    Q_INVOKABLE QString formatRawId(QString id) const;

    Enums::UserType userTypeFromString(const QString &text) const;

private:
    Utility();

    qreal setupDpiScale();

    qreal m_dpiScale;

    QString m_dateFormat = QStringLiteral("yyyy/MM/dd");
    QRegExp m_emailRegex;
};

#endif // UTILITY_H
