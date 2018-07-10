#include "actionmoredamagebyenergyonenemy.h"

ActionMoreDamageByEnergyOnEnemy::ActionMoreDamageByEnergyOnEnemy(QVariant arg) :
    AbstractAction(arg)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionMoreDamageByEnergyOnEnemy::type()
{
    return AbstractAction::Action_MoreDamageByEnergyOnEnemy;
}

bool ActionMoreDamageByEnergyOnEnemy::checkElements()
{
    bool ok;

    m_damageByEnergy = m_arg1.toInt(&ok);

    return ok;
}


void ActionMoreDamageByEnergyOnEnemy::actionAfterAttack()
{
    if((pokemonAttacking() != nullptr) && (pokemonAttacked() != nullptr))
    {
        unsigned short countEnergies = pokemonAttacked()->countEnergies();
        pokemonAttacked()->takeDamage(m_damageByEnergy * countEnergies);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionMoreDamageByEnergyOnEnemy::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked
            << AbstractAction::CheckPokemonAttacking;
}
