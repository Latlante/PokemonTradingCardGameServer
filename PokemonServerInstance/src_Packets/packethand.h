#ifndef PACKETHAND_H
#define PACKETHAND_H

#include "abstractpacketdynamic.h"

class PacketHand : public AbstractPacketDynamic
{
    Q_OBJECT

public:
    PacketHand(QList<AbstractCard *> listCards = QList<AbstractCard*>());
    virtual ~PacketHand();
	
    int maxCards() const override;

    bool isFirstHandOk();
};

#endif // PACKETHAND_H
