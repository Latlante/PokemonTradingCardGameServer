#include "packetrewards.h"

PacketRewards::PacketRewards(const QString &namePacket, QList<AbstractCard*> listCards) :
    AbstractPacketDynamic(namePacket, listCards)
{
	
}

PacketRewards::~PacketRewards()
{
	
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int PacketRewards::maxCards() const
{
    return MAXCARDS_REWARD;
}
