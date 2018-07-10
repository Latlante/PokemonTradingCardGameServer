#include "actionmoredamagebyenergy.h"
#include "gamemanager.h"
#include "src_Packets/fightarea.h"
#include "src_Cards/cardpokemon.h"

ActionMoreDamageByEnergy::ActionMoreDamageByEnergy(QVariant arg1, QVariant arg2) :
    AbstractAction(arg1, arg2)
{
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionMoreDamageByEnergy::type()
{
    return AbstractAction::Action_MoreDamageByEnergy;
}

bool ActionMoreDamageByEnergy::checkElements()
{
    bool ok;

    m_damageByEnergy = m_arg1.toInt(&ok);

    if(ok)
        m_indexOfAttack = m_arg2.toInt(&ok);

    return ok;
}

void ActionMoreDamageByEnergy::actionAfterAttack()
{
    bool status = false;
    unsigned short countEnergies = 0;

    if((pokemonAttacking() != nullptr) && (indexAttack() != -1))
    {
        AbstractCard::Enum_element elementEnergy = pokemonAttacking()->element();
        countEnergies = pokemonAttacking()->countEnergies(elementEnergy);
        countEnergies -= pokemonAttacking()->listAttacks()[indexAttack()].costEnergies[elementEnergy];
        status = true;
    }

    if(status == true)
    {
        if(pokemonAttacked() != nullptr)
        {
            pokemonAttacked()->takeDamage(m_damageByEnergy * countEnergies);
        }

    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionMoreDamageByEnergy::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked
            << AbstractAction::CheckPokemonAttacking;
}
