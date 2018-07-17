#ifndef NOTIFICATIONDATAPOKEMONCHANGED_H
#define NOTIFICATIONDATAPOKEMONCHANGED_H

#include "abstractnotification.h"

class NotificationDataPokemonChanged : public AbstractNotification
{
public:
    NotificationDataPokemonChanged(ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int lifeLeft, QMap<unsigned int, bool> mapAttacksAvailable, QList<unsigned int> listIdEnergies);

private:
    ConstantesShared::EnumPacket m_packet;
    unsigned int m_indexCard;
    unsigned int m_lifeLeft;
    QMap<unsigned int, bool> m_mapAttacksAvailable;
    QList<unsigned int> m_listIdEnergies;

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;
};

#endif // NOTIFICATIONDATAPOKEMONCHANGED_H
