#ifndef NOTIFICATIONNEWGAMECREATED_H
#define NOTIFICATIONNEWGAMECREATED_H

#include "abstractnotification.h"

class NotificationNewGameCreated : public AbstractNotification
{
public:
    explicit NotificationNewGameCreated(int uid, const QString& nameGame, const QString& namePlayer, const QString &opponent);
    virtual ~NotificationNewGameCreated() override;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

signals:

private:
    int m_uid;
    QString m_nameGame;
    QString m_opponent;
};

#endif // NOTIFICATIONNEWGAMECREATED_H
