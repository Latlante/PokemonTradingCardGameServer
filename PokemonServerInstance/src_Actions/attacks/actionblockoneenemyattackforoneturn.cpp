#include "actionblockoneenemyattackforoneturn.h"

ActionBlockOneEnemyAttackForOneTurn::ActionBlockOneEnemyAttackForOneTurn() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionBlockOneEnemyAttackForOneTurn::type()
{
    return AbstractAction::Action_BlockOneEnemyAttackForOneTurn;
}

bool ActionBlockOneEnemyAttackForOneTurn::checkElements()
{
    return true;
}

void ActionBlockOneEnemyAttackForOneTurn::actionAfterAttack()
{
    if((gameManager() != nullptr) && (pokemonAttacked() != nullptr))
    {
        unsigned short indexEnemy = gameManager()->displayAttacks(pokemonAttacked(), true);
        pokemonAttacked()->setNumberOfTurnAttackStillBlocks(indexEnemy, 1);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionBlockOneEnemyAttackForOneTurn::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckGameManager
            << AbstractAction::CheckPokemonAttacked;
}
