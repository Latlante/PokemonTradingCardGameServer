#include "bencharea.h"

#include <QDebug>
#include <QVariant>
#include "../Share/constantesshared.h"
#include "src_Cards/abstractcard.h"
#include "src_Cards/cardpokemon.h"
#include "src_Log/log.h"
#include "src_Models/modellistenergies.h"

BenchArea::BenchArea(QList<AbstractCard*> listCards) :
    AbstractPacketStatic(ConstantesShared::PACKET_Bench, NAME_BENCH, listCards)
{
	
}

BenchArea::~BenchArea()
{
	
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int BenchArea::maxCards() const
{
    return MAXCARDS_BENCH;
}

CardPokemon* BenchArea::cardPok(int index)
{
    AbstractCard* aCard = card(index);

    return dynamic_cast<CardPokemon*>(aCard);
}

ModelListEnergies* BenchArea::modelFromCardPokemon(int index)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + QString::number(index));

    ModelListEnergies* modelToReturn = nullptr;

    if((index >= 0) && (index < countCard()))
    {
        CardPokemon* cardPok = dynamic_cast<CardPokemon*>(m_listCards[index]);

        if(cardPok != nullptr)
            modelToReturn = cardPok->modelListOfEnergies();
    }

    return modelToReturn;
}
