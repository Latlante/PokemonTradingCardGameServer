#ifndef NOTIFICATIONNEWMESSAGE_H
#define NOTIFICATIONNEWMESSAGE_H

#include "abstractnotification.h"

class NotificationNewMessage : public AbstractNotification
{
    Q_OBJECT
public:
    explicit NotificationNewMessage(const QString& namePlayer, const QString& message, bool showToEveryOne = false);
    virtual ~NotificationNewMessage() override;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    QString m_message;
    bool m_showToEveryOne;
};

#endif // NOTIFICATIONNEWMESSAGE_H
