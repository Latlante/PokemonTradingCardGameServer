#ifndef NOTIFICATIONPLAYERISREADY_H
#define NOTIFICATIONPLAYERISREADY_H

#include "abstractnotification.h"

class NotificationPlayerIsReady : public AbstractNotification
{
public:
    NotificationPlayerIsReady(bool everyoneIsReady, QMap<QString,bool> mapPlayersReady);

private:
    bool m_everyoneIsReady;
    QMap<QString, bool> m_mapPlayersReady;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;
};

#endif // NOTIFICATIONPLAYERISREADY_H
