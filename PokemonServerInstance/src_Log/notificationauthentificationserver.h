#ifndef NOTIFICATIONAUTHENTIFICATIONSERVER_H
#define NOTIFICATIONAUTHENTIFICATIONSERVER_H

#include "abstractnotification.h"

class NotificationAuthentificationServer : public AbstractNotification
{
public:
    explicit NotificationAuthentificationServer(const QString& namePlayer, const QString &uidGame, const QString &nameGame, const QString &namePlayer1, const QString &namePlayer2);
    virtual ~NotificationAuthentificationServer() override;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

signals:

private:
    QString m_uidGame;
    QString m_nameGame;
    QString m_namePlayer1;
    QString m_namePlayer2;
};

#endif // NOTIFICATIONAUTHENTIFICATIONSERVER_H
