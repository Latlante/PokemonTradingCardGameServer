#ifndef NOTIFICATIONENDOFGAME_H
#define NOTIFICATIONENDOFGAME_H

#include "abstractnotification.h"

class NotificationEndOfGame : public AbstractNotification
{
    Q_OBJECT
public:
    explicit NotificationEndOfGame(const QString& namePlayerWinner, const QString& namePlayerLoser);

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    QString m_namePlayerWinner;
    QString m_namePlayerLoser;
};

#endif // NOTIFICATIONENDOFGAME_H
