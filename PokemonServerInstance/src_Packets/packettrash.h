#ifndef PACKETTRASH_H
#define PACKETTRASH_H

#include "abstractpacketdynamic.h"

class PacketTrash : public AbstractPacketDynamic
{
    Q_OBJECT

public:
    PacketTrash(QList<AbstractCard*> listCards = QList<AbstractCard*>());
    virtual ~PacketTrash();
	
    int maxCards() const override;

};

#endif // PACKETTRASH_H
