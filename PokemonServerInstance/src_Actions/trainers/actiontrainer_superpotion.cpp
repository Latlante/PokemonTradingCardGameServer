#include "actiontrainer_superpotion.h"

ActionTrainer_SuperPotion::ActionTrainer_SuperPotion() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionTrainer_SuperPotion::type()
{
    return AbstractAction::ActionTrainer_SuperPotion;
}

QList<AbstractAction::Enum_ElementsToCheck> ActionTrainer_SuperPotion::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttached;
}

void ActionTrainer_SuperPotion::actionBeforeAttack()
{
    if(pokemonAttached() != nullptr)
    {
        pokemonAttached()->restoreLife(40);
    }
}
