#include "packettrash.h"

#include "../Share/constantesshared.h"

PacketTrash::PacketTrash(QList<AbstractCard*> listCards) :
    AbstractPacketDynamic(ConstantesShared::PACKET_Trash, NAME_TRASH, listCards)
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
