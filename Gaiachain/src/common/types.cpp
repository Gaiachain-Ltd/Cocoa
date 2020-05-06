#include "types.h"

#include "globals.h"

#include <QHash>
#include <QPair>
#include <QDateTime>
#include <QDataStream>

// All of this, because Qt doesn't allow to reqister converter for non custom types

using TypesPair = QPair<QMetaType::Type, QMetaType::Type>;
using ConverterFunc = std::function<void(QVariant &)>;

namespace {
const QChar StringListDelimeter = ';';

static const QHash<TypesPair, ConverterFunc> Converters = {
    { { QMetaType::QDateTime, QMetaType::LongLong },
      [](QVariant &value) {
          auto dt = value.toDateTime();
          value.setValue(dt.toSecsSinceEpoch());
      } },
    { { QMetaType::LongLong, QMetaType::QDateTime },
      [](QVariant &value) {
          auto ll = value.toLongLong();
          value.setValue(QDateTime::fromSecsSinceEpoch(ll));
      } },

    { { QMetaType::QDate, QMetaType::LongLong },
      [](QVariant &value) {
          auto dt = QDateTime(value.toDate());
          value.setValue(dt.toSecsSinceEpoch());
      } },
    { { QMetaType::LongLong, QMetaType::QDate },
      [](QVariant &value) {
          auto ll = value.toLongLong();
          value.setValue(QDateTime::fromSecsSinceEpoch(ll).date());
      } },

    { { QMetaType::QDate, QMetaType::Double },
      [](QVariant &value) {
          auto dt = QDateTime(value.toDate());
          value.setValue(static_cast<double>(dt.toSecsSinceEpoch()));
      } },
    { { QMetaType::Double, QMetaType::QDate },
      [](QVariant &value) {
          auto d = value.toDouble();
          value.setValue(QDateTime::fromSecsSinceEpoch(static_cast<long long>(d)).date());
      } },

    { { QMetaType::QStringList, QMetaType::QString },
      [](QVariant &value) {
          auto sl = value.toStringList();
          value.setValue(sl.join(StringListDelimeter));
      } },
    { { QMetaType::QString, QMetaType::QStringList },
      [](QVariant &value) {
          auto s = value.toString();
          value.setValue(s.split(StringListDelimeter, QString::SkipEmptyParts));
      } },

    { { QMetaType::QVariantMap, QMetaType::QByteArray },
      [](QVariant &value) {
          auto vm = value.toMap();
          auto ba = QByteArray{};

          QDataStream outStream(&ba, QIODevice::WriteOnly);
          outStream << vm;

          value.setValue(ba);
      } },
    { { QMetaType::QByteArray, QMetaType::QVariantMap },
      [](QVariant &value) {
          auto ba = value.toByteArray();
          auto vm = QVariantMap{};

          QDataStream inStream(&ba, QIODevice::ReadOnly);
          inStream >> vm;

          value.setValue(vm);
      } },
};
}

bool types::canCustomConvert(const QVariant &value, const QMetaType::Type toType)
{
    return Converters.contains({ static_cast<QMetaType::Type>(value.type()), toType });
}

bool types::customConvert(QVariant &value, const QMetaType::Type toType)
{
    if (!canCustomConvert(value, toType)) {
        return false;
    }

    auto converter = Converters.value({ static_cast<QMetaType::Type>(value.type()), toType });
    converter(value);
    return true;
}

void types::convert(QVariant &value, const QMetaType::Type toType)
{
    if (!value.isNull() && toType != QMetaType::Void &&
            static_cast<QMetaType::Type>(value.type()) != toType) {
        if (!canCustomConvert(value, toType)) {
            auto intToType = static_cast<int>(toType);
            if (!value.canConvert(intToType)) {
                qCWarning(dataTypes) << "Error - cannot convert data"
                                     << value.type() << "to:" << toType;
            } else {
                value.convert(intToType);
            }
        } else {
            customConvert(value, toType);
        }
    }
}

QVariant types::convert(const QVariant &value, const QMetaType::Type toType)
{
    auto updatedValue = value;
    convert(updatedValue, toType);
    return updatedValue;
}
