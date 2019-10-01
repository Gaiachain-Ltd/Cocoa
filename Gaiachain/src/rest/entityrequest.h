#ifndef ENTITYREQUEST_H
#define ENTITYREQUEST_H

#include "baserequest.h"

#include <QElapsedTimer>

#include "../common/enums.h"

class EntityRequest : public BaseRequest
{
    Q_OBJECT
public:
    enum RequestType {
        RequestGet = 0,
        RequestGetFiltered,
        RequestBatch,
        RequestCalendar,
        RequestUninitializedGet,
        RequestUninitializedPost, // not used (web part)
        RequestEntityGet,
        RequestEntityPut
    };
    Q_ENUM(RequestType)

    EntityRequest(const QString &token, const RequestType requestType = RequestType::RequestGet);
    EntityRequest(const QString &token, const int count, const QString &type);
    EntityRequest(const QString &token, const QString &id);
    EntityRequest(const QString &token, const QJsonArray &ids);
    EntityRequest(const QString &token, const QString &id, const Enums::SupplyChainAction action, const Enums::ActionProgress actionProgress);
    EntityRequest(const QString &token, const QString &dateFrom, const QString &dateTo);

private:
    const RequestType m_requestType = RequestType::RequestGet;
    QElapsedTimer m_timer;

protected:
    virtual void parse() Q_DECL_OVERRIDE final;
};


#endif // ENTITYREQUEST_H
