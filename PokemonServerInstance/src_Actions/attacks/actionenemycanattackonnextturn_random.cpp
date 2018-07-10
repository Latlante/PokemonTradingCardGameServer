#include "actionenemycanattackonnextturn_random.h"

ActionEnemyCanAttackOnNextTurn_Random::ActionEnemyCanAttackOnNextTurn_Random() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionEnemyCanAttackOnNextTurn_Random::type()
{
    return AbstractAction::Action_EnemyCanAttackOnNextTurn_Random;
}

bool ActionEnemyCanAttackOnNextTurn_Random::checkElements()
{
    return true;
}

void ActionEnemyCanAttackOnNextTurn_Random::actionAfterAttack()
{
    unsigned short coin = headOrTail();

    if((gameManager() != nullptr) && (pokemonAttacked() != nullptr) && (coin == 1))
    {
        for(int i=0;i<pokemonAttacked()->listAttacks().count();++i)
        {
            pokemonAttacked()->setNumberOfTurnAttackStillBlocks(i, 1);
        }
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionEnemyCanAttackOnNextTurn_Random::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckGameManager
            << AbstractAction::CheckPokemonAttacked;
}
