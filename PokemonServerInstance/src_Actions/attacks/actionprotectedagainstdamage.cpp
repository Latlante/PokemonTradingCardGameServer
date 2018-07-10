#include "actionprotectedagainstdamage.h"

ActionProtectedAgainstDamage::ActionProtectedAgainstDamage() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionProtectedAgainstDamage::type()
{
    return AbstractAction::Action_ProtectedAgainstDamage;
}

bool ActionProtectedAgainstDamage::checkElements()
{
    return true;
}

void ActionProtectedAgainstDamage::actionAfterAttack()
{
    if(pokemonAttacking() != nullptr)
        pokemonAttacking()->setProtectedAgainstDamageForTheNextTurn(true);
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionProtectedAgainstDamage::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacking;
}
