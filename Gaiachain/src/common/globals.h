#ifndef GLOBALS_H
#define GLOBALS_H

#include <QVector>
#include <QObject>

#if defined(Q_OS_LINUX) and !defined(Q_OS_ANDROID)
    #define Q_OS_LINUX_DESKTOP 1
#endif

#define SERVER_ADDRESS QStringLiteral("https://beta.gaiachain.io/api/v1")
#define QR_CODE_LENGTH 10

namespace Gaia {
    using ModelData = QVector<QVariantList>;
}

#endif // GLOBALS_H
