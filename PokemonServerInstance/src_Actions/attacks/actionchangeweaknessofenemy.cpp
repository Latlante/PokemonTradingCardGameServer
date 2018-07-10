#include "actionchangeweaknessofenemy.h"

#include "src_Cards/cardenergy.h"

ActionChangeWeaknessOfEnemy::ActionChangeWeaknessOfEnemy() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionChangeWeaknessOfEnemy::type()
{
    return AbstractAction::Action_ChangeWeaknessOfEnemy;
}

bool ActionChangeWeaknessOfEnemy::checkElements()
{
    return true;
}

void ActionChangeWeaknessOfEnemy::actionAfterAttack()
{
    if((gameManager() != nullptr) && (pokemonAttacked() != nullptr))
    {
        AbstractCard::Enum_element element = gameManager()->displayAllElements(1);
        pokemonAttacked()->setWeaknessElement(element);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionChangeWeaknessOfEnemy::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckGameManager
            << AbstractAction::CheckPokemonAttacked;
}
