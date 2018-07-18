#ifndef NOTIFICATIONPLAYERISREADY_H
#define NOTIFICATIONPLAYERISREADY_H

#include "abstractnotification.h"
#include <QMap>

class NotificationPlayerIsReady : public AbstractNotification
{
public:
    NotificationPlayerIsReady(bool everyoneIsReady, QMap<QString,bool> mapPlayersReady);

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    bool m_everyoneIsReady;
    QMap<QString, bool> m_mapPlayersReady;
};

#endif // NOTIFICATIONPLAYERISREADY_H
