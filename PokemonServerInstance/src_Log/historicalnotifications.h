#ifndef HISTORICALNOTIFICATIONS_H
#define HISTORICALNOTIFICATIONS_H

#include <QObject>

#include "src_Log/notificationcardmoved.h"
#include "src_Log/notificationdatapokemonchanged.h"
#include "src_Log/notificationendofturn.h"
#include "src_Log/notificationenergyadded.h"
#include "src_Log/notificationenergyremoved.h"
#include "src_Log/notificationplayerisready.h"
#include "src_Log/notificationpokemonswitched.h"

#include "src_Log/notificationdisplaypackets.h"

class AbstractNotification;

class HistoricalNotifications
{
public:
    explicit HistoricalNotifications();

    int addNewNotification(AbstractNotification* notif);
    unsigned int readPoint();
    QJsonObject buildJsonOwnerFrom(unsigned int index);
    QJsonObject buildJsonOthersFrom(unsigned int index);

signals:

private:
    QList<AbstractNotification*> m_listNotifications;
};

#endif // HISTORICALNOTIFICATIONS_H
