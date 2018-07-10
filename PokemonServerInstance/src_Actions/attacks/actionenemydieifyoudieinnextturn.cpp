#include "actionenemydieifyoudieinnextturn.h"

ActionEnemyDieIfYouDieInNextTurn::ActionEnemyDieIfYouDieInNextTurn() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionEnemyDieIfYouDieInNextTurn::type()
{
    return AbstractAction::Action_EnemyDieIfYouDieInNextTurn;
}

bool ActionEnemyDieIfYouDieInNextTurn::checkElements()
{
    return true;
}

void ActionEnemyDieIfYouDieInNextTurn::actionAfterAttack()
{
    if(pokemonAttacking() != nullptr)
    {
        pokemonAttacking()->setDestinyBond(true);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionEnemyDieIfYouDieInNextTurn::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacking;
}
