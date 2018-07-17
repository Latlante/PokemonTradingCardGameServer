#ifndef NOTIFICATIONENDOFTURN_H
#define NOTIFICATIONENDOFTURN_H

#include "abstractnotification.h"

class NotificationEndOfTurn : public AbstractNotification
{
public:
    NotificationEndOfTurn(const QString& oldPlayer, const QString& newPlayer);

private:
    QString m_nameOldPlayer;
    QString m_nameNewPlayer;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;
};

#endif // NOTIFICATIONENDOFTURN_H
