#include "actiontrainer_restored.h"

#include "src_Packets/packethand.h"
#include "src_Packets/packettrash.h"

ActionTrainer_Restored::ActionTrainer_Restored()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionTrainer_Restored::type()
{
    return AbstractAction::ActionTrainer_Restored;
}

QList<AbstractAction::Enum_ElementsToCheck> ActionTrainer_Restored::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckGameManager
            << AbstractAction::CheckPlayerAttacking;
}

void ActionTrainer_Restored::actionBeforeAttack()
{
    if((gameManager() != nullptr) && (playerAttacking() != nullptr))
    {
        //On défausse 2 cartes de la main
        QList<AbstractCard*> listCardsHand = gameManager()->displayPacket(playerAttacking()->hand(), 2);
        foreach(AbstractCard* card, listCardsHand)
            playerAttacking()->moveCardFromHandToTrash(card);

        //On récupére une carte dresseur de la pile de défausse
        QList<AbstractCard*> listCardsTrash = gameManager()->displayPacket(playerAttacking()->trash(), 1, AbstractCard::TypeOfCard_Action);
        foreach(AbstractCard* card, listCardsTrash)
            playerAttacking()->moveCardFromTrashToHand(card);
    }
}
