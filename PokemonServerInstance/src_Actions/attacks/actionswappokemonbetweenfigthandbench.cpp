#include "actionswappokemonbetweenfigthandbench.h"

#include "src_Log/log.h"

ActionSwapPokemonBetweenFigthAndBench::ActionSwapPokemonBetweenFigthAndBench() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionSwapPokemonBetweenFigthAndBench::type()
{
    return AbstractAction::Action_SwapPokemonBetweenFigthAndBench;
}

bool ActionSwapPokemonBetweenFigthAndBench::checkElements()
{
    return true;
}

void ActionSwapPokemonBetweenFigthAndBench::actionAfterAttack()
{
    if((pokemonAttacked() != nullptr) && (benchPlayerAttacked() != nullptr))
    {
        if(benchPlayerAttacked()->countCard() > 0)
        {
            QList<AbstractCard*> listPokemonChoose = gameManager()->displayPacket(benchPlayerAttacked());

            if((listPokemonChoose.count() >= 1) && (listPokemonChoose.first()->type() == AbstractCard::TypeOfCard_Pokemon))
                playerAttacked()->swapCardsBetweenBenchAndFight(static_cast<CardPokemon*>(listPokemonChoose.first()));
        }
        else
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + " Bench is empty");
    }
    else
    {
        QString messageError = "Element is/are nullptr:\n";
        messageError += pokemonAttacked() == nullptr ? "  - err: pokemonAttacked is nullptr\n" : "  - pokemonAttacked " + pokemonAttacked()->name();
        messageError += benchPlayerAttacked() == nullptr ? "  - err: benchPlayerAttacked is nullptr\n" : "  - benchPlayerAttacked " + benchPlayerAttacked()->name();

        Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + messageError);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionSwapPokemonBetweenFigthAndBench::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPlayerAttacked
            << AbstractAction::CheckBenchPlayerAttacked;
}
