#ifndef NOTIFICATIONENERGYREMOVED_H
#define NOTIFICATIONENERGYREMOVED_H

#include "abstractnotification.h"

class NotificationEnergyRemoved : public AbstractNotification
{
public:
    NotificationEnergyRemoved(const QString &namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, unsigned int indexEnergy);

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    ConstantesShared::EnumPacket m_packetOrigin, m_packetDestination;
    unsigned int m_indexCardOrigin, m_indexCardDestination;
    unsigned int m_indexEnergy;
};

#endif // NOTIFICATIONENERGYREMOVED_H
