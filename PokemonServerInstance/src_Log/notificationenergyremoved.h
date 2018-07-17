#ifndef NOTIFICATIONENERGYREMOVED_H
#define NOTIFICATIONENERGYREMOVED_H

#include "abstractnotification.h"

class NotificationEnergyRemoved : public AbstractNotification
{
public:
    NotificationEnergyRemoved(ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int indexEnergy);

private:
    ConstantesShared::EnumPacket m_packet;
    unsigned int m_indexCard;
    unsigned int m_indexEnergy;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;
};

#endif // NOTIFICATIONENERGYREMOVED_H
