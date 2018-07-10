#include "actiontrainer_potion.h"

ActionTrainer_Potion::ActionTrainer_Potion() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionTrainer_Potion::type()
{
    return AbstractAction::ActionTrainer_Potion;
}

QList<AbstractAction::Enum_ElementsToCheck> ActionTrainer_Potion::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttached;
}

void ActionTrainer_Potion::actionBeforeAttack()
{
    if(pokemonAttached() != nullptr)
    {
        pokemonAttached()->restoreLife(20);
    }
}
