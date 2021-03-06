#include "utility.h"

#include <QGuiApplication>
#include <QScreen>
#include <QColor>

#include "../common/logs.h"
#include "../common/globals.h"

Utility &Utility::instance()
{
    static Utility u;
    return u;
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
    dpiScale = QGuiApplication::primaryScreen()->physicalDotsPerInch() / (332.5 * 0.55);
#endif

#ifdef Q_OS_IOS
    dpiScale = QGuiApplication::primaryScreen()->logicalDotsPerInch() / QGuiApplication::primaryScreen()->physicalDotsPerInch();
#endif

    return dpiScale;
}

QString Utility::formatRawId(QString id) const
{
    if (!id.contains('-')) {
        for (int i = QR_CODE_LENGTH - 4 ; i > 0 ; i-=4)
            id.insert(i, '-');
    }
    return id;
}

QString Utility::colorString(const QColor &color) const
{
    if (color.isValid() == false) {
        return QString();
    }

    return color.name(QColor::HexArgb);
}

QString Utility::allUpperCase(const QString &input) const
{
    return input.toUpper();
}

QVariantMap Utility::createSummaryItem(
    const QString &headerText,
    const QVariant &value,
    const QString &inputIconSource,
    const QString &suffix,
    const QColor &highlightColor,
    const QColor &decorationColor,
    const QColor &secondaryTextColor,
    const Enums::DelegateType delegateType,
    const bool hasUnderline) const
{
    //qDebug() << "Data is:" << headerText << value << inputIconSource << suffix << highlightColor << decorationColor << secondaryTextColor << delegateType;

    return {
        { "headerValue", headerText },
        { "value", value },
        { "inputIconSource", inputIconSource },
        { "suffixValue", suffix },
        { "delegateType", int(delegateType) },
        { "highlightColor", highlightColor },
        { "decorationColor", decorationColor },
        { "secondaryTextColor", secondaryTextColor },
        { "isHighlighted", (highlightColor.isValid() == true) },
        { "hasSecondaryTextColor", secondaryTextColor.isValid() },
        { "hasUnderline", hasUnderline }
    };
}

QVariantMap Utility::createSummaryItem(
    const QString &headerText,
    const SummaryValue &value,
    const QString &inputIconSource,
    const QString &suffix,
    const QColor &highlightColor,
    const QColor &decorationColor,
    const QColor &secondaryTextColor,
    const Enums::DelegateType delegateType,
    const bool hasUnderline) const
{
    return createSummaryItem(headerText, value.toList(), inputIconSource,
                             suffix, highlightColor, decorationColor,
                             secondaryTextColor, delegateType,
                             hasUnderline);
}

bool Utility::isWeekend(const QDate &date) const
{
    const int day = date.dayOfWeek();
    // TODO: Sat and Sun are not weekend days in all countries...
    return day == 6 || day == 7;
}

QDate Utility::previousMonth(const QDate &date) const
{
    return date.addMonths(-1);
}

QDate Utility::nextMonth(const QDate &date) const
{
    return date.addMonths(1);
}

int Utility::parseInt(const QString &num) const
{
    return num.toInt();
}

/*!
 * Returns a \a number with constant number of \a digits.
 */
QString Utility::constDigitsNumber(const int number, const int digits) const
{
    QString result(QString::number(number));

    if (result.size() > digits) {
        qWarning() << Q_FUNC_INFO << "Number is too big!" << number << digits;
    }

    while (result.size() < digits) {
        result.prepend("0");
    }

    return result;
}

bool Utility::validateId(const QString &id) const
{
    QString rawId = id;
    rawId.remove('-');
    QString::const_iterator it = rawId.constBegin();
    while(it != rawId.constEnd()) {
        const QChar &c = (*it);
        if (c.isLetterOrNumber() == false)
            return false;
        ++it;
    }
    return rawId.length() == QR_CODE_LENGTH;
}

bool Utility::validateEmail(const QString &email) const
{
    return m_emailRegex.exactMatch(email);
}

int Utility::getScannedIdLength() const
{
    return QR_CODE_LENGTH;
}

/*!
 * Converts a \a list into a map which is then returned. JS will interpret the
 * map as an object.
 *
 * \a list elements are inserted in pairs:
 \code
  let arr = [ "key", "value", "anotherKey", ["another", "values"]]
  let obj = Utility.arrayToObject(arr)
  // obj is:
  // {
  //    key: "value",
  //    anotherKey: ["another", "values]
  // }
 \endcode
 */
QVariantMap Utility::arrayToObject(const QVariantList &list) const
{
    const int size = list.size();
    QVariantMap result;
    if ((size % 2) != 0) {
        qWarning() << "Odd-sized list" << size
                   <<"cannot be converted into an object!" << list;
        return result;
    }

    QString key;
    for (int i = 0; i < size; ++i) {
        if (i % 2) {
            result.insert(key, list.at(i));
            key.clear();
        } else {
            key = list.at(i).toString();
        }
    }

    return result;
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

QVariantList Utility::SummaryValue::toList() const
{
    const int length = titles.length();
    if (length != values.length() || length != icons.length()) {
        qWarning() << RED("Incorrect value lengths in SummaryValue!")
                   << length << values.length() << icons.length()
                   << linkDatas.length();
    }

    if (linkDestinationPages.length() != linkDatas.length()) {
        qWarning() << RED("Link destinations and contents do not match")
                   << linkDestinationPages.length() << linkDatas.length();
    }

    QStringList destinations;
    for (const auto &page : qAsConst(linkDestinationPages)) {
        destinations.append(QString::number(int(page)));
    }

    return {
        titles,
        values,
        icons,
        destinations,
        linkDatas
    };
}
