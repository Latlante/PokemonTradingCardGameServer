#include "actionchangeenemystatus.h"

#include "src_Log/log.h"

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
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", type=" + QString::number(type()) + ", status=" + QString::number(m_status));
#endif

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
