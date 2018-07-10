#include "actionhealing.h"

ActionHealing::ActionHealing(QVariant arg1, QVariant arg2) :
    AbstractAction(arg1, arg2)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionHealing::type()
{
    return AbstractAction::Action_Healing;
}

bool ActionHealing::checkElements()
{
    bool ok;

    m_pv = m_arg1.toInt(&ok);

    if(ok)
        m_energyToPay = static_cast<AbstractCard::Enum_element>(m_arg2.toInt(&ok));

    return ok;
}

void ActionHealing::actionBeforeAttack()
{
    if(pokemonAttacking() != nullptr)
    {
        if(m_energyToPay != AbstractCard::Element_None)
        {


        }

        pokemonAttacking()->restoreLife(m_pv);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionHealing::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacking;
}
