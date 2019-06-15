#include "packethand.h"
#include <QDebug>

#include "src_Cards/abstractcard.h"
#include "src_Cards/cardpokemon.h"
#include "../Share/constantesshared.h"

PacketHand::PacketHand(QList<AbstractCard*> listCards) :
    AbstractPacketDynamic(ConstantesShared::PACKET_Hand, NAME_HAND, listCards)
{
	
}

PacketHand::~PacketHand()
{
	
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int PacketHand::maxCards() const
{
    return MAXCARDS_HAND;
}

//La main doit contenir au moins un pokémon de base
bool PacketHand::isFirstHandOk()
{
    bool atLeastOneBaseFound = false;
    int index = 0;

    while((index < countCard()) && (atLeastOneBaseFound == false))
    {
        AbstractCard* aCard = card(index);
        if(aCard->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* pokemon = static_cast<CardPokemon*>(aCard);

            if(pokemon->isBase())
                atLeastOneBaseFound = true;
        }

        index++;
    }

    return atLeastOneBaseFound;
}
