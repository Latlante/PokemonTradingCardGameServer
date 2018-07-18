#ifndef NOTIFICATIONENDOFTURN_H
#define NOTIFICATIONENDOFTURN_H

#include "abstractnotification.h"

class NotificationEndOfTurn : public AbstractNotification
{
public:
    NotificationEndOfTurn(const QString& oldPlayer, const QString& newPlayer);

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    QString m_nameOldPlayer;
    QString m_nameNewPlayer;
};

#endif // NOTIFICATIONENDOFTURN_H
