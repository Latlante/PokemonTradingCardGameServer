#ifndef NOTIFICATIONENERGYADDED_H
#define NOTIFICATIONENERGYADDED_H

#include "abstractnotification.h"

class NotificationEnergyAdded : public AbstractNotification
{
public:
    NotificationEnergyAdded(ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int idEnergy);

private:
    ConstantesShared::EnumPacket m_packet;
    unsigned int m_indexCard;
    unsigned int m_idEnergy;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;
};

#endif // NOTIFICATIONENERGYADDED_H
