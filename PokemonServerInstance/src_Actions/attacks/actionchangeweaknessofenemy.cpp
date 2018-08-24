#include "actionchangeweaknessofenemy.h"

#include "src_Cards/cardenergy.h"
#include "src_Log/log.h"

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
        QList<AbstractCard::Enum_element> elements = gameManager()->displayAllElements(1);
        if(elements.count() > 0)
            pokemonAttacked()->setWeaknessElement(elements[0]);
        else
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + " list of elements has only " + QString::number(elements.count()) + " elements");
    }
    else
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + " gameManager or pokemonAttacked is null");
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
