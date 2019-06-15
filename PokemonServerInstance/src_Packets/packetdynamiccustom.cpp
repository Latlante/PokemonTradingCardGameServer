#include "packetdynamiccustom.h"

#include "src_Cards/abstractcard.h"

PacketDynamicCustom::PacketDynamicCustom(const QString &namePacket, QList<AbstractCard *> listCards) :
    AbstractPacketDynamic(0, namePacket, listCards),
    m_maxCards(-1)
{

}
/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int PacketDynamicCustom::maxCards() const
{
    return m_maxCards;
}

void PacketDynamicCustom::setMaxCards(int max)
{
    m_maxCards = max;
}
