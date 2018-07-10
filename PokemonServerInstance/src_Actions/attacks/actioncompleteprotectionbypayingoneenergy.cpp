#include "actioncompleteprotectionbypayingoneenergy.h"

ActionCompleteProtectionByPayingOneEnergy::ActionCompleteProtectionByPayingOneEnergy(QVariant arg) :
    AbstractAction(arg)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionCompleteProtectionByPayingOneEnergy::type()
{
    return AbstractAction::Action_CompleteProtectionByPayingOneEnergy;
}

bool ActionCompleteProtectionByPayingOneEnergy::checkElements()
{
    bool ok;

    m_element = static_cast<AbstractCard::Enum_element>(m_arg1.toInt(&ok));

    return ok;
}

void ActionCompleteProtectionByPayingOneEnergy::actionBeforeAttack()
{
    if((gameManager() != nullptr) && (pokemonAttacking() != nullptr))
    {
        QList<CardEnergy*> listEnergies = gameManager()->displayEnergiesForAPokemon(pokemonAttacking(), 1, m_element);

        if(listEnergies.count() >= 1)
        {
            pokemonAttacking()->moveEnergiesInTrash(listEnergies);
            pokemonAttacking()->setInvincibleForTheNextTurn(true);
        }
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionCompleteProtectionByPayingOneEnergy::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckGameManager
            << AbstractAction::CheckPokemonAttacking;
}
