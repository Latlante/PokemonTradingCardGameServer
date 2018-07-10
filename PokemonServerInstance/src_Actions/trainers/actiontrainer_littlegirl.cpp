#include "actiontrainer_littlegirl.h"

ActionTrainer_LittleGirl::ActionTrainer_LittleGirl() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionTrainer_LittleGirl::type()
{
    return AbstractAction::ActionTrainer_LittleGirl;
}

QList<AbstractAction::Enum_ElementsToCheck> ActionTrainer_LittleGirl::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckGameManager
            << AbstractAction::CheckPlayerAttacking;
}

void ActionTrainer_LittleGirl::actionBeforeAttack()
{
    if((gameManager() != nullptr) && (playerAttacking() != nullptr))
    {

    }
}
