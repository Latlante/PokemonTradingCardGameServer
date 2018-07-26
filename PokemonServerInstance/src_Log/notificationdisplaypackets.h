#ifndef NOTIFICATIONDISPLAYPACKETS_H
#define NOTIFICATIONDISPLAYPACKETS_H

#include "abstractnotification.h"
#include <QMap>

class NotificationDisplayPackets : public AbstractNotification
{
public:
    NotificationDisplayPackets(const QString& namePlayer, QMap<int,int> cards);
    virtual ~NotificationDisplayPackets() override;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    QMap<int,int> m_cards;
};

#endif // NOTIFICATIONDISPLAYPACKETS_H
