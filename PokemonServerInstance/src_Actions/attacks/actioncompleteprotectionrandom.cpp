#include "actioncompleteprotectionrandom.h"

ActionCompleteProtectionRandom::ActionCompleteProtectionRandom() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionCompleteProtectionRandom::type()
{
    return AbstractAction::Action_CompleteProtectionRandom;
}

bool ActionCompleteProtectionRandom::checkElements()
{
    return true;
}

void ActionCompleteProtectionRandom::actionAfterAttack()
{
    if(pokemonAttacking() != nullptr)
    {
        unsigned short coin = headOrTail();

        if(coin == 1)
            pokemonAttacking()->setInvincibleForTheNextTurn(true);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionCompleteProtectionRandom::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacking;
}
