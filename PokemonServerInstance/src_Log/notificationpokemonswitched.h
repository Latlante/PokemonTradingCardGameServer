#ifndef NOTIFICATIONPOKEMONSWITCHED_H
#define NOTIFICATIONPOKEMONSWITCHED_H

#include "abstractnotification.h"

class NotificationPokemonSwitched : public AbstractNotification
{
public:
    NotificationPokemonSwitched(ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int newIdCard);

private:
    ConstantesShared::EnumPacket m_packet;
    unsigned int m_indexCard;
    unsigned int m_newIdCard;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;
};

#endif // NOTIFICATIONPOKEMONSWITCHED_H
