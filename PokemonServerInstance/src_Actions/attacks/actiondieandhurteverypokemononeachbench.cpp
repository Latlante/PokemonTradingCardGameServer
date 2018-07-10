#include "actiondieandhurteverypokemononeachbench.h"

ActionDieAndHurtEveryPokemonOnEachBench::ActionDieAndHurtEveryPokemonOnEachBench(QVariant arg) :
    AbstractAction(arg)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionDieAndHurtEveryPokemonOnEachBench::type()
{
    return AbstractAction::Action_DieAndHurtEveryPokemonOnEachBench;
}

bool ActionDieAndHurtEveryPokemonOnEachBench::checkElements()
{
    bool ok;

    m_damage = m_arg1.toInt(&ok);

    return ok;
}

void ActionDieAndHurtEveryPokemonOnEachBench::actionAfterAttack()
{
    if(pokemonAttacking() != nullptr)
        pokemonAttacking()->takeDamage(pokemonAttacking()->lifeTotal());

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

    if(benchPlayerAttacked() != nullptr)
    {
        for(int i=0;i<benchPlayerAttacked()->countCard();++i)
        {
            AbstractCard* abCard = benchPlayerAttacked()->card(i);

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
QList<AbstractAction::Enum_ElementsToCheck> ActionDieAndHurtEveryPokemonOnEachBench::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacking
            << AbstractAction::CheckBenchPlayerAttacking
            << AbstractAction::CheckBenchPlayerAttacked;
}
