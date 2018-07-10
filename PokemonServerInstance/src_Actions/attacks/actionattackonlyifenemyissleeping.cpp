#include "actionattackonlyifenemyissleeping.h"

ActionAttackOnlyIfEnemyIsSleeping::ActionAttackOnlyIfEnemyIsSleeping(QVariant arg1) :
    AbstractAction(arg1)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionAttackOnlyIfEnemyIsSleeping::type()
{
    return AbstractAction::Action_AttackOnlyIfEnemyIsSleeping;
}

bool ActionAttackOnlyIfEnemyIsSleeping::checkElements()
{
    bool ok;

    m_damage = m_arg1.toInt(&ok);

    return ok;
}

void ActionAttackOnlyIfEnemyIsSleeping::actionAfterAttack()
{
    if((pokemonAttacked() != nullptr) && (pokemonAttacked()->status() == CardPokemon::Status_Slept))
        pokemonAttacked()->takeDamage(m_damage);
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionAttackOnlyIfEnemyIsSleeping::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked;
}
