#include "actionmoredamageorhurthimself.h"

ActionMoreDamageOrHurtHimSelf::ActionMoreDamageOrHurtHimSelf(QVariant arg1, QVariant arg2) :
    AbstractAction(arg1, arg2)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionMoreDamageOrHurtHimSelf::type()
{
    return AbstractAction::Action_MoreDamageOrHurtHimSelf;
}

bool ActionMoreDamageOrHurtHimSelf::checkElements()
{
    bool ok;

    m_damageOnHead = m_arg1.toInt(&ok);

    if(ok)
        m_damageOnTail = m_arg2.toInt(&ok);

    return ok;
}

void ActionMoreDamageOrHurtHimSelf::actionAfterAttack()
{
    unsigned short coin = headOrTail();

    //Hurt the enemy
    if(coin == 1)
    {
        if(pokemonAttacked() != nullptr)
        {
            pokemonAttacked()->takeDamage(m_damageOnHead);
        }
    }
    //Hurt himself
    else
    {
        if(pokemonAttacking() != nullptr)
        {
            pokemonAttacking()->takeDamage(m_damageOnTail);
        }
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionMoreDamageOrHurtHimSelf::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked
            << AbstractAction::CheckPokemonAttacking;
}
