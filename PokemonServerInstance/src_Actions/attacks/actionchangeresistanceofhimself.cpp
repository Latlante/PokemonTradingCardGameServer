#include "actionchangeresistanceofhimself.h"

ActionChangeResistanceOfHimself::ActionChangeResistanceOfHimself() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionChangeResistanceOfHimself::type()
{
    return AbstractAction::Action_ChangeResistanceOfHimself;
}

bool ActionChangeResistanceOfHimself::checkElements()
{
    return true;
}

void ActionChangeResistanceOfHimself::actionAfterAttack()
{
    if((gameManager() != nullptr) && (pokemonAttacking() != nullptr))
    {
        AbstractCard::Enum_element element = gameManager()->displayAllElements(1);
        pokemonAttacking()->setResistanceElement(element);
    }
    else
        qCritical() << __PRETTY_FUNCTION__ << "Element null";
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionChangeResistanceOfHimself::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckGameManager
            << AbstractAction::CheckPokemonAttacking;
}
