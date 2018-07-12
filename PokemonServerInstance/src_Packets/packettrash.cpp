#include "packettrash.h"

PacketTrash::PacketTrash(const QString &namePacket, QList<AbstractCard*> listCards) :
    AbstractPacketDynamic(namePacket, listCards)
{
	
}

PacketTrash::~PacketTrash()
{
	
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int PacketTrash::maxCards() const
{
    return MAXCARDS_TRASH;
}
