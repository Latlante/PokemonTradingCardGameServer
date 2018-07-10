#include "actiontrainer_informaticsearch.h"

#include "src_Packets/packetdeck.h"
#include "src_Packets/packethand.h"

ActionTrainer_InformaticSearch::ActionTrainer_InformaticSearch() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionTrainer_InformaticSearch::type()
{
    return AbstractAction::ActionTrainer_InformaticSearch;
}

QList<AbstractAction::Enum_ElementsToCheck> ActionTrainer_InformaticSearch::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckGameManager
            << AbstractAction::CheckPlayerAttacking;
}

void ActionTrainer_InformaticSearch::actionBeforeAttack()
{
    if((gameManager() != nullptr) && (playerAttacking() != nullptr))
    {
        //On défausse 2 cartes de la main
        QList<AbstractCard*> listCardsHand = gameManager()->displayPacket(playerAttacking()->hand(), 2);
        foreach(AbstractCard* card, listCardsHand)
            playerAttacking()->moveCardFromHandToTrash(card);

        //On récupére une carte de notre deck
        QList<AbstractCard*> listCardsDeck = gameManager()->displayPacket(playerAttacking()->deck(), 1);
        foreach(AbstractCard* card, listCardsDeck)
            playerAttacking()->moveCardFromDeckToHand(card);

        //On remélange le deck
        playerAttacking()->deck()->mixCards();
    }
}
