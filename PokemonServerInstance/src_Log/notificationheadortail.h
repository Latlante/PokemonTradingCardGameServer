#ifndef NOTIFICATIONHEADORTAIL_H
#define NOTIFICATIONHEADORTAIL_H

#include "abstractnotification.h"

class NotificationHeadOrTail : public AbstractNotification
{
public:
    explicit NotificationHeadOrTail(const QString &namePlayer, QList<unsigned short> headsOrTails);
    virtual ~NotificationHeadOrTail() override;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    QList<unsigned short> m_headsOrTails;
};

#endif // NOTIFICATIONHEADORTAIL_H
