#include "actionchangeenemystatusrandom.h"

#include "src_Log/log.h"

ActionChangeEnemyStatusRandom::ActionChangeEnemyStatusRandom(QVariant arg1, QVariant arg2) :
    AbstractAction(arg1, arg2)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionChangeEnemyStatusRandom::type()
{
    return AbstractAction::Action_ChangeEnemyStatus_Random;
}

bool ActionChangeEnemyStatusRandom::checkElements()
{
    bool ok;

    m_statusOnHead = static_cast<CardPokemon::Enum_statusOfPokemon>(m_arg1.toInt(&ok));

    if(ok)
        m_statusOnTail = static_cast<CardPokemon::Enum_statusOfPokemon>(m_arg2.toInt(&ok));

    return ok;
}

void ActionChangeEnemyStatusRandom::actionAfterAttack()
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", type=" + QString::number(type()) + ", statusOnHead=" + QString::number(m_statusOnHead) + ", statusOnTail=" + QString::number(m_statusOnTail));
#endif

    if(pokemonAttacked() != nullptr)
    {
        if(headOrTail() == 1)
            pokemonAttacked()->setStatus(m_statusOnHead);
        else
            pokemonAttacked()->setStatus(m_statusOnTail);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionChangeEnemyStatusRandom::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked;
}
