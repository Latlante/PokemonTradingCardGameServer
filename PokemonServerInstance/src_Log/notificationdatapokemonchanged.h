#ifndef NOTIFICATIONDATAPOKEMONCHANGED_H
#define NOTIFICATIONDATAPOKEMONCHANGED_H

#include "abstractnotification.h"
#include <QMap>

class NotificationDataPokemonChanged : public AbstractNotification
{
public:
    NotificationDataPokemonChanged(const QString& namePlayer,
                                   ConstantesShared::EnumPacket packet,
                                   unsigned int indexCard,
                                   unsigned int lifeLeft,
                                   QMap<unsigned int, bool> mapAttacksAvailable,
                                   QList<unsigned int> listIdEnergies);

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    ConstantesShared::EnumPacket m_packet;
    unsigned int m_indexCard;
    unsigned int m_lifeLeft;
    QMap<unsigned int, bool> m_mapAttacksAvailable;
    QList<unsigned int> m_listIdEnergies;
};

#endif // NOTIFICATIONDATAPOKEMONCHANGED_H
