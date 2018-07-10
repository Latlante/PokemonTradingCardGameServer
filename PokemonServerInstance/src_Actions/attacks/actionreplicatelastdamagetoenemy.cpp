#include "actionreplicatelastdamagetoenemy.h"

ActionReplicateLastDamageToEnemy::ActionReplicateLastDamageToEnemy() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionReplicateLastDamageToEnemy::type()
{
    return AbstractAction::Action_ReplicateLastDamageToEnemy;
}

bool ActionReplicateLastDamageToEnemy::checkElements()
{
    return true;
}

void ActionReplicateLastDamageToEnemy::actionAfterAttack()
{
    if((pokemonAttacked() != nullptr) && (pokemonAttacking()))
        pokemonAttacked()->takeDamage(pokemonAttacking()->lastDamageReceived());
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionReplicateLastDamageToEnemy::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked
            << AbstractAction::CheckPokemonAttacking;
}
