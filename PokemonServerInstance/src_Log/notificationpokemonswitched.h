#ifndef NOTIFICATIONPOKEMONSWITCHED_H
#define NOTIFICATIONPOKEMONSWITCHED_H

#include "abstractnotification.h"

class NotificationPokemonSwitched : public AbstractNotification
{
public:
    NotificationPokemonSwitched(const QString& namePlayer, ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int newIdCard, bool keepEnergy);

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    ConstantesShared::EnumPacket m_packet;
    unsigned int m_indexCard;
    unsigned int m_newIdCard;
    bool m_keepEnergy;
};

#endif // NOTIFICATIONPOKEMONSWITCHED_H
