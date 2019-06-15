#ifndef PACKETREWARDS_H
#define PACKETREWARDS_H

#include "abstractpacketdynamic.h"

class PacketRewards : public AbstractPacketDynamic
{
    Q_OBJECT

public:
    PacketRewards(QList<AbstractCard*> listCards = QList<AbstractCard*>());
    virtual ~PacketRewards();
	
    int maxCards() const override;

};

#endif // PACKETREWARDS_H
