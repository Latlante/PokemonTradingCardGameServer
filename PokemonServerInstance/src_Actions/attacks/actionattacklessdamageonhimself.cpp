#include "actionattacklessdamageonhimself.h"

ActionAttackLessDamageOnHimself::ActionAttackLessDamageOnHimself(QVariant arg1, QVariant arg2) :
    AbstractAction(arg1, arg2)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionAttackLessDamageOnHimself::type()
{
    return AbstractAction::Action_AttackLessDamageOnHimself;
}

bool ActionAttackLessDamageOnHimself::checkElements()
{
    bool ok;

    m_originalDamage = m_arg1.toInt(&ok);

    if(ok)
        m_damagePerMarquer = m_arg2.toInt(&ok);

    return ok;
}

void ActionAttackLessDamageOnHimself::actionAfterAttack()
{
    if((pokemonAttacking() != nullptr) && (pokemonAttacked() != nullptr))
    {
        unsigned short damageMarquer = pokemonAttacking()->damageMarker();
        unsigned short damageToMake = 0;
        unsigned short ownDamage = damageMarquer * m_damagePerMarquer;

        if(m_originalDamage > ownDamage)
            damageToMake = m_originalDamage - ownDamage;

        pokemonAttacked()->takeDamage(damageToMake);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionAttackLessDamageOnHimself::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked
            << AbstractAction::CheckPokemonAttacking;
}
