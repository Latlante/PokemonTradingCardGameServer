#include "actionhurteverypokemononownbench.h"

ActionHurtEveryPokemonOnOwnBench::ActionHurtEveryPokemonOnOwnBench(QVariant arg) :
    AbstractAction(arg)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionHurtEveryPokemonOnOwnBench::type()
{
    return AbstractAction::Action_HurtEveryPokemonOnOwnBench;
}

bool ActionHurtEveryPokemonOnOwnBench::checkElements()
{
    bool ok;

    m_damage = m_arg1.toInt(&ok);

    return ok;
}

void ActionHurtEveryPokemonOnOwnBench::actionAfterAttack()
{
    if(benchPlayerAttacking() != nullptr)
    {
        for(int i=0;i<benchPlayerAttacking()->countCard();++i)
        {
            AbstractCard* abCard = benchPlayerAttacking()->card(i);

            if(abCard->type() == CardPokemon::TypeOfCard_Pokemon)
            {
                CardPokemon* cardPok = static_cast<CardPokemon*>(abCard);

                if(cardPok != nullptr)
                {
                    cardPok->takeDamage(m_damage);
                }
            }
        }
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionHurtEveryPokemonOnOwnBench::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckBenchPlayerAttacking;
}
