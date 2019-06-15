#include "fightarea.h"

#include <QDebug>
#include "src_Cards/cardpokemon.h"
#include "src_Log/log.h"
#include "common/utils.h"
#include "../Share/constantesshared.h"

FightArea::FightArea(QList<AbstractCard*> listCards) :
    AbstractPacketStatic(ConstantesShared::PACKET_Fight, NAME_FIGHT, listCards)
{
    connect(this, &FightArea::countChanged, this, &FightArea::onCountChanged);
}

FightArea::~FightArea()
{
	
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int FightArea::maxCards() const
{
    return MAXCARDS_FIGHT;
}

CardPokemon* FightArea::pokemonFighter()
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__));
#endif

    return pokemonFighting(0);
}

CardPokemon* FightArea::pokemonFighting(int index)
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__));
#endif

    CardPokemon* pokemon = NULL;

    AbstractCard* abCard = card(index);

    if(abCard != NULL)
    {
        pokemon = static_cast<CardPokemon*>(abCard);
    }

    return pokemon;
}

/************************************************************
*****			FONCTIONS SLOTS PRIVEES					*****
************************************************************/
void FightArea::onCountChanged()
{
    emit pokemonFighterChanged();
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/

