#include "actiontrainer_removeenergy.h"

ActionTrainer_RemoveEnergy::ActionTrainer_RemoveEnergy() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionTrainer_RemoveEnergy::type()
{
    return AbstractAction::ActionTrainer_RemoveEnergy;
}

QList<AbstractAction::Enum_ElementsToCheck> ActionTrainer_RemoveEnergy::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckGameManager
            << AbstractAction::CheckPokemonAttacked;
}

void ActionTrainer_RemoveEnergy::actionBeforeAttack()
{
    if((gameManager() != nullptr) && (pokemonAttacked() != nullptr))
    {
        QList<CardEnergy*> listEnergies = gameManager()->displayEnergiesForAPokemon(pokemonAttacked(), 1, AbstractCard::Element_Whatever);
        pokemonAttacked()->moveEnergiesInTrash(listEnergies);
    }
}
