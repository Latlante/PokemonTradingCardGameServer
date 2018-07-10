#include "actionenemypoisoned.h"

ActionEnemyPoisoned::ActionEnemyPoisoned(QVariant arg) :
    AbstractAction(arg)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionEnemyPoisoned::type()
{
    return AbstractAction::Action_EnemyPoisoned;
}

bool ActionEnemyPoisoned::checkElements()
{
    bool ok;

    m_damage = m_arg1.toInt(&ok);

    return ok;
}

void ActionEnemyPoisoned::actionAfterAttack()
{
    if(pokemonAttacked() != nullptr)
    {
        pokemonAttacked()->setDamagePoisonPerRound(m_damage);
        pokemonAttacked()->setStatus(CardPokemon::Status_Poisoned);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionEnemyPoisoned::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked;
}
