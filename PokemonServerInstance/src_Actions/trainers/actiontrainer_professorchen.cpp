#include "actiontrainer_professorchen.h"

#include "src_Packets/packethand.h"

ActionTrainer_ProfessorChen::ActionTrainer_ProfessorChen()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionTrainer_ProfessorChen::type()
{
    return AbstractAction::ActionTrainer_ProfessorChen;
}

QList<AbstractAction::Enum_ElementsToCheck> ActionTrainer_ProfessorChen::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPlayerAttacking;
}

void ActionTrainer_ProfessorChen::actionBeforeAttack()
{
    if(playerAttacking() != nullptr)
    {
        //On nettoie la main
        while(playerAttacking()->hand()->countCard() > 0)
            playerAttacking()->moveCardFromHandToTrash(playerAttacking()->hand()->card(0));

        //Et on pioche les cartes
        for(int i=0;i<7;++i)
            playerAttacking()->drawOneCard();
    }
}
