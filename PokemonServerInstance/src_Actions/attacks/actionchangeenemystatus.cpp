#include "actionchangeenemystatus.h"

ActionChangeEnemyStatus::ActionChangeEnemyStatus(QVariant arg1) :
    AbstractAction(arg1)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionChangeEnemyStatus::type()
{
    return AbstractAction::Action_ChangeEnemyStatus;
}

bool ActionChangeEnemyStatus::checkElements()
{
    bool ok;

    m_status = static_cast<CardPokemon::Enum_statusOfPokemon>(m_arg1.toInt(&ok));

    return ok;
}

void ActionChangeEnemyStatus::actionAfterAttack()
{
    if(pokemonAttacked() != nullptr)
        pokemonAttacked()->setStatus(m_status);
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionChangeEnemyStatus::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked;
}
