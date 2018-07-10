#include "actionmoredamagebyenemydamage.h"

ActionMoreDamageByEnemyDamage::ActionMoreDamageByEnemyDamage(QVariant arg) :
    AbstractAction(arg)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionMoreDamageByEnemyDamage::type()
{
    return AbstractAction::Action_MoreDamageByEnemyDamage;
}

bool ActionMoreDamageByEnemyDamage::checkElements()
{
    bool ok;

    m_damage = m_arg1.toInt(&ok);

    return ok;
}

void ActionMoreDamageByEnemyDamage::actionBeforeAttack()
{
    if(pokemonAttacked() != nullptr)
    {
        unsigned short damageMarquer = pokemonAttacked()->damageMarker();
        pokemonAttacked()->takeDamage(damageMarquer * m_damage);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionMoreDamageByEnemyDamage::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked
            << AbstractAction::CheckPokemonAttacking;
}
