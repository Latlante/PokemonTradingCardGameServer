#include "packetrewards.h"

#include "../Share/constantesshared.h"

PacketRewards::PacketRewards(QList<AbstractCard*> listCards) :
    AbstractPacketDynamic(ConstantesShared::PACKET_Rewards, NAME_REWARDS, listCards)
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
