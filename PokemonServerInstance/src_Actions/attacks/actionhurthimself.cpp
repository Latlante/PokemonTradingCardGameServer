#include "actionhurthimself.h"

ActionHurtHimself::ActionHurtHimself(QVariant arg) :
    AbstractAction(arg)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionHurtHimself::type()
{
    return AbstractAction::Action_HurtHimself;
}

bool ActionHurtHimself::checkElements()
{
    bool ok;

    m_damage = m_arg1.toInt(&ok);

    return ok;
}

void ActionHurtHimself::actionAfterAttack()
{
    if(pokemonAttacking() != nullptr)
        pokemonAttacking()->takeDamage(m_damage);
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionHurtHimself::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacking;
}
