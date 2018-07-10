#include "actionremoveoneenergyonenemy.h"

#include "gamemanager.h"
#include "src_Packets/fightarea.h"
#include "src_Packets/packettrash.h"
#include "src_Cards/cardenergy.h"
#include "src_Cards/cardpokemon.h"

ActionRemoveOneEnergyOnEnemy::ActionRemoveOneEnergyOnEnemy(QVariant arg1, QVariant arg2) :
    AbstractAction(arg1, arg2)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionRemoveOneEnergyOnEnemy::type()
{
    return AbstractAction::Action_RemoveOneEnergyOnEnemy;
}

bool ActionRemoveOneEnergyOnEnemy::checkElements()
{
    bool ok;

    m_numberOfEnergiesToRemoved = m_arg1.toInt(&ok);

    if(ok)
        m_elementToRemove = static_cast<AbstractCard::Enum_element>(m_arg2.toInt(&ok));

    return ok;
}

void ActionRemoveOneEnergyOnEnemy::actionAfterAttack()
{
    if(pokemonAttacked() != nullptr)
    {
        //Si on a plus d'énergies que demandé, on choisit lesquels on veut enlever
        if(pokemonAttacked()->countEnergies() > m_numberOfEnergiesToRemoved)
        {
            QList<CardEnergy*> listEnergies = gameManager()->displayEnergiesForAPokemon(pokemonAttacked(), m_numberOfEnergiesToRemoved, m_elementToRemove);
            pokemonAttacked()->moveEnergiesInTrash(listEnergies);
        }
        //Sinon on enléve tout
        else
        {
            pokemonAttacked()->moveAllEnergiesInTrash();
        }
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionRemoveOneEnergyOnEnemy::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked;
}
