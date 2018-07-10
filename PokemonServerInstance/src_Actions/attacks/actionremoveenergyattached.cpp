#include "actionremoveenergyattached.h"

#include "gamemanager.h"
#include "src_Packets/fightarea.h"
#include "src_Packets/packettrash.h"
#include "src_Cards/cardenergy.h"
#include "src_Cards/cardpokemon.h"

ActionRemoveEnergyAttached::ActionRemoveEnergyAttached(QVariant arg1, QVariant arg2) :
    AbstractAction(arg1, arg2)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionRemoveEnergyAttached::type()
{
    return AbstractAction::Action_RemoveEnergyAttached;
}

bool ActionRemoveEnergyAttached::checkElements()
{
    bool ok;

    m_numberOfEnergiesToRemoved = m_arg1.toInt(&ok);

    if(ok)
        m_elementToRemove = static_cast<AbstractCard::Enum_element>(m_arg2.toInt(&ok));

    return ok;
}

void ActionRemoveEnergyAttached::actionBeforeAttack()
{
    if(pokemonAttacking() != nullptr)
    {
        //Si on a plus d'énergies que demandé, on choisit lesquels on veut enlever
        if(pokemonAttacking()->countEnergies() > m_numberOfEnergiesToRemoved)
        {
            QList<CardEnergy*> listEnergies = gameManager()->displayEnergiesForAPokemon(pokemonAttacking(), m_numberOfEnergiesToRemoved, m_elementToRemove);
            pokemonAttacking()->moveEnergiesInTrash(listEnergies);
        }
        //Sinon on enléve tout
        else
        {
            pokemonAttacking()->moveAllEnergiesInTrash();
        }
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionRemoveEnergyAttached::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckGameManager
            << AbstractAction::CheckPokemonAttacking;
}
