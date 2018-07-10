#include "actionprotectedagainstdamagerandom.h"

ActionProtectedAgainstDamageRandom::ActionProtectedAgainstDamageRandom() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionProtectedAgainstDamageRandom::type()
{
    return AbstractAction::Action_ProtectedAgainstDamageRandom;
}

bool ActionProtectedAgainstDamageRandom::checkElements()
{
    return true;
}

void ActionProtectedAgainstDamageRandom::actionAfterAttack()
{
    if(pokemonAttacking() != nullptr)
    {
        if(headOrTail() == 1)
            pokemonAttacking()->setProtectedAgainstDamageForTheNextTurn(true);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionProtectedAgainstDamageRandom::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacking;
}
