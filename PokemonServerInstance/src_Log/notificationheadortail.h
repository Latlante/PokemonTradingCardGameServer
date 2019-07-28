#ifndef NOTIFICATIONHEADORTAIL_H
#define NOTIFICATIONHEADORTAIL_H

#include "abstractnotification.h"

class NotificationHeadOrTail : public AbstractNotification
{
    Q_OBJECT
public:
    explicit NotificationHeadOrTail(const QString &namePlayer, bool headOrTail);
    virtual ~NotificationHeadOrTail() override;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    bool m_headOrTail;
};

#endif // NOTIFICATIONHEADORTAIL_H
