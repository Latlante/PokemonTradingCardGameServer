#include "actionreplicateoneattackfromenemy.h"

#include "src_Log/log.h"

ActionReplicateOneAttackFromEnemy::ActionReplicateOneAttackFromEnemy() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionReplicateOneAttackFromEnemy::type()
{
    return AbstractAction::Action_ReplicateOneAttackFromEnemy;
}

bool ActionReplicateOneAttackFromEnemy::checkElements()
{
    return true;
}

void ActionReplicateOneAttackFromEnemy::actionAfterAttack()
{
    if(pokemonAttacked() != nullptr)
    {
        int indexAttackToReplicate = gameManager()->displayAttacks(pokemonAttacked(), true);
        int indexLastAttack = pokemonAttacking()->lastIndexOfAttackUsed();

        if((indexAttackToReplicate >= 0) && (indexLastAttack >= 0) && (pokemonAttacking() != nullptr))
        {
            pokemonAttacking()->replaceOneAttack(indexLastAttack,
                                                 pokemonAttacked()->listAttacks()[indexAttackToReplicate]);
        }
        else
            Log::instance()->write(QString(__PRETTY_FUNCTION__)
                        + " Impossible d'exécuter l'action, indexAttackToReplicate="
                        + QString::number(indexAttackToReplicate)
                        + ", indexLastAttack="
                        + QString::number(indexLastAttack));
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionReplicateOneAttackFromEnemy::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckGameManager
            << AbstractAction::CheckPokemonAttacked
            << AbstractAction::CheckPokemonAttacking;
}
