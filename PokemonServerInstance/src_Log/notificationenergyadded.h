#ifndef NOTIFICATIONENERGYADDED_H
#define NOTIFICATIONENERGYADDED_H

#include "abstractnotification.h"

class NotificationEnergyAdded : public AbstractNotification
{
public:
    NotificationEnergyAdded(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, unsigned int idEnergy);

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    ConstantesShared::EnumPacket m_packetOrigin, m_packetDestination;
    unsigned int m_indexCardOrigin, m_indexCardDestination;
    unsigned int m_idEnergy;
};

#endif // NOTIFICATIONENERGYADDED_H
