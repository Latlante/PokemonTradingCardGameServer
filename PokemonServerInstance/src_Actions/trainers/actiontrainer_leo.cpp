#include "actiontrainer_leo.h"

ActionTrainer_Leo::ActionTrainer_Leo() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionTrainer_Leo::type()
{
    return AbstractAction::ActionTrainer_Leo;
}

QList<AbstractAction::Enum_ElementsToCheck> ActionTrainer_Leo::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPlayerAttacking;
}

void ActionTrainer_Leo::actionBeforeAttack()
{
    if(playerAttacking() != nullptr)
    {
        for(int i=0;i<2;++i)
        {
            playerAttacking()->drawOneCard();
        }
    }
}
