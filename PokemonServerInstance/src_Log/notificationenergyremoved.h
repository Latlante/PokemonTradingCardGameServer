#ifndef NOTIFICATIONENERGYREMOVED_H
#define NOTIFICATIONENERGYREMOVED_H

#include "abstractnotification.h"

class NotificationEnergyRemoved : public AbstractNotification
{
public:
    NotificationEnergyRemoved(const QString &namePlayer, ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int indexEnergy);

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    ConstantesShared::EnumPacket m_packet;
    unsigned int m_indexCard;
    unsigned int m_indexEnergy;
};

#endif // NOTIFICATIONENERGYREMOVED_H
