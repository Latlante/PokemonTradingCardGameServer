#include "actionnodamageonthreshold.h"

ActionNoDamageOnThreshold::ActionNoDamageOnThreshold(QVariant arg) :
    AbstractAction(arg)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionNoDamageOnThreshold::type()
{
    return AbstractAction::Action_NoDamageOnThreshold;
}

bool ActionNoDamageOnThreshold::checkElements()
{
    bool ok;

    m_threshold = m_arg1.toInt(&ok);

    return ok;
}

void ActionNoDamageOnThreshold::actionAfterAttack()
{
    if(pokemonAttacking() != nullptr)
        pokemonAttacking()->setProtectedAgainstDamageForTheNextTurn(m_threshold);
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionNoDamageOnThreshold::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacking;
}
