#include "actionmoredamagebyowndamage.h"

ActionMoreDamageByOwnDamage::ActionMoreDamageByOwnDamage(QVariant arg) :
    AbstractAction(arg)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionMoreDamageByOwnDamage::type()
{
    return AbstractAction::Action_MoreDamageByOwnDamage;
}

bool ActionMoreDamageByOwnDamage::checkElements()
{
    bool ok;

    m_damage = m_arg1.toInt(&ok);

    return ok;
}

void ActionMoreDamageByOwnDamage::actionAfterAttack()
{
    if(pokemonAttacked() != nullptr)
    {
        unsigned short damageMarquer = pokemonAttacking()->damageMarker();
        pokemonAttacked()->takeDamage(damageMarquer * m_damage);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionMoreDamageByOwnDamage::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked
            << AbstractAction::CheckPokemonAttacking;
}
