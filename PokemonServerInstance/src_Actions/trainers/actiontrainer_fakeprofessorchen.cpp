#include "actiontrainer_fakeprofessorchen.h"

#include "src_Packets/packethand.h"

ActionTrainer_FakeProfessorChen::ActionTrainer_FakeProfessorChen() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionTrainer_FakeProfessorChen::type()
{
    return AbstractAction::ActionTrainer_FakeProfessorChen;
}

QList<AbstractAction::Enum_ElementsToCheck> ActionTrainer_FakeProfessorChen::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPlayerAttacked;
}

void ActionTrainer_FakeProfessorChen::actionBeforeAttack()
{
    if(playerAttacked() != nullptr)
    {
        //On nettoie la main
        while(playerAttacked()->hand()->countCard() > 0)
            playerAttacked()->moveCardFromHandToTrash(playerAttacked()->hand()->card(0));

        for(int i=0;i<7;++i)
            playerAttacked()->drawOneCard();
    }
}
