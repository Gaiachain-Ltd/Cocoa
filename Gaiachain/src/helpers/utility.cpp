#include "utility.h"

#include <QGuiApplication>
#include <QScreen>

#include "../common/globals.h"

Utility *Utility::instance()
{
    static Utility u;
    return &u;
}

qreal Utility::scaleByDpi(qreal num) const
{
    return num * m_dpiScale;
}

qreal Utility::scaleRoundByDpi(qreal num) const
{
    if (num < 0 && num >= -5) {
        const qreal res = qRound(scaleByDpi(num));
        return std::min(res, -1.0);
    } else if (num > 0 && num <= 5) {
        const qreal res = qRound(scaleByDpi(num));
        return std::max(res, 1.0);
    } else {
        return scaleByDpi(num);
    }
}

qreal Utility::proportionalWidth(qreal val, qreal refWidth) const
{
    if (refWidth < 0.0)
        refWidth = m_refWidth;

    qreal prop = clamp(val, 0.0, refWidth) / refWidth;
    return prop * refWidth;
}

qreal Utility::proportionalHeight(qreal val, qreal refHeight) const
{
    if (refHeight < 0.0)
        refHeight = m_refHeight;

    qreal prop = clamp(val, 0.0, refHeight) / refHeight;
    return prop * refHeight;
}

qreal Utility::clamp(qreal v, qreal min, qreal max) const
{
    return qMin(max, qMax(min, v));
}

Utility::Utility()
{
    m_emailRegex = QRegExp(QStringLiteral("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b"));
    m_emailRegex.setCaseSensitivity(Qt::CaseInsensitive);
    m_emailRegex.setPatternSyntax(QRegExp::RegExp);

    m_dpiScale = setupDpiScale();
}

qreal Utility::setupDpiScale()
{
    // 332.5 is the reference dpi (BlackBerry Q5 / Q10).
    qreal dpiScale = QGuiApplication::primaryScreen()->physicalDotsPerInch() / 332.5;

#ifdef Q_OS_LINUX_DESKTOP
    dpiScale = QGuiApplication::primaryScreen()->physicalDotsPerInch() / (332.5 * 0.6);
#endif

#ifdef Q_OS_IOS
    dpiScale = QGuiApplication::primaryScreen()->logicalDotsPerInch() / QGuiApplication::primaryScreen()->physicalDotsPerInch();
#endif

    return dpiScale;
}

int Utility::parseInt(const QString &num) const
{
    return num.toInt();
}

bool Utility::validateId(const QString &id) const
{
    bool ok;
    id.toInt(&ok);
    return ok && id.length() == QR_CODE_LENGTH;
}

int Utility::getScannedIdLength() const
{
    return QR_CODE_LENGTH;
}

bool Utility::validateEmail(const QString &email) const
{
    return m_emailRegex.exactMatch(email);
}

QDate Utility::convertDateString(const QString &dateStr, const QString &dateFormat) const
{
    QString format;
    if (dateFormat.isEmpty() || dateFormat.isNull()) {
        format = defaultDateFormat();
    } else {
        format = dateFormat;
    }

    return QDate::fromString(dateStr, format);
}

QString Utility::defaultDateFormat() const
{
    return m_dateFormat;
}
