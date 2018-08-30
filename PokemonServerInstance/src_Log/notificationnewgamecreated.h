#ifndef NOTIFICATIONNEWGAMECREATED_H
#define NOTIFICATIONNEWGAMECREATED_H

#include "abstractnotification.h"

class NotificationNewGameCreated : public AbstractNotification
{
    Q_OBJECT
public:
    explicit NotificationNewGameCreated(const QString& namePlayer, const QString &opponent);
    virtual ~NotificationNewGameCreated() override;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

signals:

private:
    QString m_opponent;
};

#endif // NOTIFICATIONNEWGAMECREATED_H
