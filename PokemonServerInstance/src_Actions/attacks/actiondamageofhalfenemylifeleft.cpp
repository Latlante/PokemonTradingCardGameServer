#include "actiondamageofhalfenemylifeleft.h"

ActionDamageOfHalfEnemyLifeLeft::ActionDamageOfHalfEnemyLifeLeft() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionDamageOfHalfEnemyLifeLeft::type()
{
    return AbstractAction::Action_DamageOfHalfEnemyLifeLeft;
}

bool ActionDamageOfHalfEnemyLifeLeft::checkElements()
{
    return true;
}

void ActionDamageOfHalfEnemyLifeLeft::actionAfterAttack()
{
    if(pokemonAttacked() != nullptr)
    {
        unsigned short lifeLeftOnEnemy = pokemonAttacked()->lifeLeft();
        unsigned short damageToMake = lifeLeftOnEnemy / 2;

        if((damageToMake % 10) == 5)
            damageToMake -= 5;

        pokemonAttacked()->takeDamage(damageToMake);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionDamageOfHalfEnemyLifeLeft::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked
            << AbstractAction::CheckPokemonAttacking;
}
