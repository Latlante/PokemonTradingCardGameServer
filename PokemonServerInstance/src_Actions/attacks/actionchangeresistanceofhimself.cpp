#include "actionchangeresistanceofhimself.h"

#include "src_Log/log.h"

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
        QList<AbstractCard::Enum_element> elements = gameManager()->displayAllElements(1);
        if(elements.count() > 0)
            pokemonAttacking()->setResistanceElement(elements[0]);
        else
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + " list of elements has only " + QString::number(elements.count()) + " elements");
    }
    else
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + " gameManager or pokemonAttacking is null");
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
