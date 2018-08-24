#include "actioncreationfactory.h"

#include <QVariant>

#include "src_Actions/attacks/actionchangeweaknessofenemy.h"
#include "src_Actions/attacks/actionchangeresistanceofhimself.h"
#include "src_Actions/attacks/actionenemydieifyoudieinnextturn.h"

#include "src_Actions/attacks/actionreplicateoneattackfromenemy.h"
#include "src_Actions/attacks/actionreplicatelastdamagetoenemy.h"
#include "src_Actions/attacks/actionuniqueattack_random.h"
#include "src_Actions/attacks/actionattackonlyifenemyissleeping.h"
#include "src_Actions/attacks/actionblockoneenemyattackforoneturn.h"
#include "src_Actions/attacks/actionenemycanattackonnextturn_random.h"

#include "src_Actions/attacks/actionmoredamagebyenergy.h"
#include "src_Actions/attacks/actiondamagemultipliedbyheadortail.h"
#include "src_Actions/attacks/actionmoredamagebyenemydamage.h"
#include "src_Actions/attacks/actionmoredamagebyowndamage.h"
#include "src_Actions/attacks/actionattacklessdamageonhimself.h"
#include "src_Actions/attacks/actiondamageofhalfenemylifeleft.h"
#include "src_Actions/attacks/actionhurthimself.h"
#include "src_Actions/attacks/actionmoredamageorhurthimself.h"
#include "src_Actions/attacks/actionmoredamagebyenergyonenemy.h"

#include "src_Actions/attacks/actionremoveenergyattached.h"
#include "src_Actions/attacks/actionremoveoneenergyonenemy.h"

#include "src_Actions/attacks/actionhealing.h"

#include "src_Actions/attacks/actionprotectedagainstdamage.h"
#include "src_Actions/attacks/actionprotectedagainstdamagerandom.h"
#include "src_Actions/attacks/actioncompleteprotectionbypayingoneenergy.h"
#include "src_Actions/attacks/actionnodamageonthreshold.h"
#include "src_Actions/attacks/actioncompleteprotectionrandom.h"

#include "src_Actions/attacks/actionchangeenemystatus.h"
#include "src_Actions/attacks/actionchangeenemystatusrandom.h"
#include "src_Actions/attacks/actionenemypoisoned.h"

#include "src_Actions/attacks/actionhurteverypokemononownbench.h"
#include "src_Actions/attacks/actiondieandhurteverypokemononeachbench.h"
#include "src_Actions/attacks/actionswappokemonbetweenfigthandbench.h"

#include "src_Actions/trainers/actiontrainer_informaticsearch.h"
#include "src_Actions/trainers/actiontrainer_fakeprofessorchen.h"
#include "src_Actions/trainers/actiontrainer_restored.h"
#include "src_Actions/trainers/actiontrainer_saveenergies.h"
#include "src_Actions/trainers/actiontrainer_totalguerison.h"
#include "src_Actions/trainers/actiontrainer_pokemoncenter.h"
#include "src_Actions/trainers/actiontrainer_professorchen.h"
#include "src_Actions/trainers/actiontrainer_superpotion.h"
#include "src_Actions/trainers/actiontrainer_leo.h"
#include "src_Actions/trainers/actiontrainer_removeenergy.h"
#include "src_Actions/trainers/actiontrainer_potion.h"

#include "src_Log/log.h"

ActionCreationFactory::ActionCreationFactory()
{

}

AbstractAction* ActionCreationFactory::createAction(AbstractAction::Enum_typeOfAction idAction, QVariant arg1, QVariant arg2)
{
    AbstractAction* actionToReturn = nullptr;

    switch(idAction)
    {
    //GENERAL
    case AbstractAction::Action_None:
        break;
    case AbstractAction::Action_ChangeWeaknessOfEnemy:
        return new ActionChangeWeaknessOfEnemy();
        break;
    case AbstractAction::Action_ChangeResistanceOfHimself:
        return new ActionChangeResistanceOfHimself();
        break;
    case AbstractAction::Action_EnemyDieIfYouDieInNextTurn:
        return new ActionEnemyDieIfYouDieInNextTurn();
        break;


    //ATTAQUES
    case AbstractAction::Action_ReplicateOneAttackFromEnemy:
        return new ActionReplicateOneAttackFromEnemy();
        break;
    case AbstractAction::Action_ReplicateLastDamageToEnemy:
        return new ActionReplicateLastDamageToEnemy();
        break;
    case AbstractAction::Action_UniqueAttack_Random:
        return new ActionUniqueAttack_Random();
        break;
    case AbstractAction::Action_AttackOnlyIfEnemyIsSleeping:
        return new ActionAttackOnlyIfEnemyIsSleeping(arg1);
        break;
    case AbstractAction::Action_BlockOneEnemyAttackForOneTurn:
        return new ActionBlockOneEnemyAttackForOneTurn();
        break;
    case AbstractAction::Action_EnemyCanAttackOnNextTurn_Random:
        return new ActionEnemyCanAttackOnNextTurn_Random();
        break;


    //GESTION DES DEGATS
    case AbstractAction::Action_MoreDamageByEnergy:
        return new ActionMoreDamageByEnergy(arg1, arg2);
        break;
    case AbstractAction::Action_DamageMultipliedByHeadOrTail:
        return new ActionDamageMultipliedByHeadOrTail(arg1, arg2);
        break;
    case AbstractAction::Action_MoreDamageByOwnDamage:
        return new ActionMoreDamageByOwnDamage(arg1);
        break;
    case AbstractAction::Action_AttackLessDamageOnHimself:
        return new ActionAttackLessDamageOnHimself(arg1, arg2);
        break;
    case AbstractAction::Action_MoreDamageByEnemyDamage:
        return new ActionMoreDamageByEnemyDamage(arg1);
        break;
    case AbstractAction::Action_DamageOfHalfEnemyLifeLeft:
        return new ActionDamageOfHalfEnemyLifeLeft();
        break;
    case AbstractAction::Action_HurtHimself:
        return new ActionHurtHimself(arg1);
        break;
    case AbstractAction::Action_MoreDamageOrHurtHimSelf:
        return new ActionMoreDamageOrHurtHimSelf(arg1, arg2);
        break;
    case AbstractAction::Action_MoreDamageByEnergyOnEnemy:
        return new ActionMoreDamageByEnergyOnEnemy(arg1);
        break;

    //ENERGIES
    case AbstractAction::Action_RemoveEnergyAttached:
        return new ActionRemoveEnergyAttached(arg1, arg2);
        break;
    case AbstractAction::Action_RemoveOneEnergyOnEnemy:
        return new ActionRemoveOneEnergyOnEnemy(arg1, arg2);
        break;

    //SOIN
    case AbstractAction::Action_Healing:
        return new ActionHealing(arg1, arg2);
        break;

    //PROTECTION
    case AbstractAction::Action_ProtectedAgainstDamage:
        actionToReturn = new ActionProtectedAgainstDamage();
        break;
    case AbstractAction::Action_ProtectedAgainstDamageRandom:
        actionToReturn = new ActionProtectedAgainstDamageRandom();
        break;
    case AbstractAction::Action_CompleteProtectionByPayingOneEnergy:
        actionToReturn = new ActionCompleteProtectionByPayingOneEnergy(arg1);
        break;
    case AbstractAction::Action_NoDamageOnThreshold:
        actionToReturn = new ActionNoDamageOnThreshold(arg1);
        break;
    case AbstractAction::Action_CompleteProtectionRandom:
        actionToReturn = new ActionCompleteProtectionRandom();
        break;

    //STATUS
    case AbstractAction::Action_ChangeEnemyStatus:
        actionToReturn = new ActionChangeEnemyStatus(arg1);
        break;
    case AbstractAction::Action_ChangeEnemyStatus_Random:
        actionToReturn = new ActionChangeEnemyStatusRandom(arg1, arg2);
        break;
    case AbstractAction::Action_EnemyPoisoned:
        actionToReturn = new ActionEnemyPoisoned(arg1);
        break;

    //BANC
    case AbstractAction::Action_HurtEveryPokemonOnOwnBench:
        actionToReturn = new ActionHurtEveryPokemonOnOwnBench(arg1);
        break;
    case AbstractAction::Action_DieAndHurtEveryPokemonOnEachBench:
        actionToReturn = new ActionDieAndHurtEveryPokemonOnEachBench(arg1);
        break;
    case AbstractAction::Action_SwapPokemonBetweenFigthAndBench:
        actionToReturn = new ActionSwapPokemonBetweenFigthAndBench();
        break;

    //ACTIONS TRAINERS
    case AbstractAction::ActionTrainer_InformaticSearch:
        actionToReturn = new ActionTrainer_InformaticSearch();
        break;
    case AbstractAction::ActionTrainer_FakeProfessorChen:
        actionToReturn = new ActionTrainer_FakeProfessorChen();
        break;
    case AbstractAction::ActionTrainer_Restored:
        actionToReturn = new ActionTrainer_Restored();
        break;
    case AbstractAction::ActionTrainer_SaveEnergies:
        actionToReturn = new ActionTrainer_SaveEnergies();
        break;
    case AbstractAction::ActionTrainer_TotalGuerison:
        actionToReturn = new ActionTrainer_TotalGuerison();
        break;
    case AbstractAction::ActionTrainer_PokemonCenter:
        actionToReturn = new ActionTrainer_PokemonCenter();
        break;
    case AbstractAction::ActionTrainer_ProfessorChen:
        actionToReturn = new ActionTrainer_ProfessorChen();
        break;
    case AbstractAction::ActionTrainer_SuperPotion:
        actionToReturn = new ActionTrainer_SuperPotion();
        break;
    case AbstractAction::ActionTrainer_Leo:
        actionToReturn = new ActionTrainer_Leo();
        break;
    case AbstractAction::ActionTrainer_RemoveEnergy:
        actionToReturn = new ActionTrainer_RemoveEnergy();
        break;
    case AbstractAction::ActionTrainer_Potion:
        actionToReturn = new ActionTrainer_Potion();
        break;
    default:
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + QString::number(idAction) + " is not ready yet");
        break;
    }

    return actionToReturn;
}

//GENERAL
/*ActionChangeWeaknessOfEnemy* ActionCreationFactory::createActionChangeWeaknessOfEnemy(AbstractCard::Enum_element element)
{
    return new ActionChangeWeaknessOfEnemy(element);
}

ActionChangeResistanceOfHimself* ActionCreationFactory::createActionChangeResistanceOfHimself(AbstractCard::Enum_element element)
{
    return new ActionChangeResistanceOfHimself(element);
}

//ATTAQUES
ActionReplicateOneAttackFromEnemy* ActionCreationFactory::createActionReplicateOneAttackFromEnemy()
{
    return new ActionReplicateOneAttackFromEnemy();
}

ActionReplicateLastDamageToEnemy* ActionCreationFactory::createActionReplicateLastDamageToEnemy()
{
    return new ActionReplicateLastDamageToEnemy();
}

ActionUniqueAttack_Random* ActionCreationFactory::createActionUniqueAttack_Random(unsigned short index)
{
    return new ActionUniqueAttack_Random(index);
}

ActionAttackOnlyIfEnemyIsSleeping* ActionCreationFactory::createActionAttackOnlyIfEnemyIsSleeping(unsigned short damage)
{
    return new ActionAttackOnlyIfEnemyIsSleeping(damage);
}

ActionBlockOneEnemyAttackForOneTurn* ActionCreationFactory::createActionBlockOneEnemyAttackForOneTurn()
{
    return new ActionBlockOneEnemyAttackForOneTurn();
}

ActionEnemyCanAttackOnNextTurn_Random* ActionCreationFactory::createActionEnemyCanAttackOnNextTurn_Random()
{
    return new ActionEnemyCanAttackOnNextTurn_Random();
}

//GESTION DES DEGATS
ActionMoreDamageByEnergy* ActionCreationFactory::createActionMoreDamageByEnergy(unsigned short damagePerEnergy, unsigned short indexOfAttack)
{
    return new ActionMoreDamageByEnergy(damagePerEnergy, indexOfAttack);
}

ActionDamageMultipliedByHeadOrTail* ActionCreationFactory::createActionDamageMultipliedByHeadOrTail(unsigned short damagePerHead, unsigned short numberOfCoins)
{
    return new ActionDamageMultipliedByHeadOrTail(damagePerHead, numberOfCoins);
}

ActionMoreDamageByEnemyDamage* ActionCreationFactory::createActionMoreDamageByEnemyDamage(unsigned short damagePerMarquer)
{
    return new ActionMoreDamageByEnemyDamage(damagePerMarquer);
}

ActionMoreDamageByOwnDamage* ActionCreationFactory::createActionMoreDamageByOwnDamage(unsigned short damage)
{
    return new ActionMoreDamageByOwnDamage(damage);
}

ActionAttackLessDamageOnHimself* ActionCreationFactory::createActionAttackLessDamageOnHimself(unsigned short originalDamage, unsigned short damagePerMarquer)
{
    return new ActionAttackLessDamageOnHimself(originalDamage, damagePerMarquer);
}

ActionDamageOfHalfEnemyLifeLeft* ActionCreationFactory::createActionDamageOfHalfEnemyLifeLeft()
{
    return new ActionDamageOfHalfEnemyLifeLeft();
}

ActionHurtHimself* ActionCreationFactory::createActionHurtHimself(unsigned short damage)
{
    return new ActionHurtHimself(damage);
}

ActionMoreDamageOrHurtHimSelf* ActionCreationFactory::createActionMoreDamageOrHurtHimSelf(unsigned short damageOnHead, unsigned short damageOnTail)
{
    return new ActionMoreDamageOrHurtHimSelf(damageOnHead, damageOnTail);
}

ActionMoreDamageByEnergyOnEnemy* ActionCreationFactory::createActionMoreDamageByEnergyOnEnemy(unsigned short damagePerEnergy)
{
    return new ActionMoreDamageByEnergyOnEnemy(damagePerEnergy);
}

//ENERGIES
ActionRemoveEnergyAttached* ActionCreationFactory::createActionRemoveEnergyAttached(unsigned short numberOfEnergies, AbstractCard::Enum_element elementToRemove)
{
    return new ActionRemoveEnergyAttached(numberOfEnergies, elementToRemove);
}

ActionRemoveOneEnergyOnEnemy* ActionCreationFactory::createActionRemoveOneEnergyOnEnemy(unsigned short numberOfEnergies, AbstractCard::Enum_element elementToRemove)
{
    return new ActionRemoveOneEnergyOnEnemy(numberOfEnergies, elementToRemove);
}

//SOIN
ActionHealing* ActionCreationFactory::createActionHealing(unsigned short pv, AbstractCard::Enum_element energyToPay)
{
    return new ActionHealing(pv, energyToPay);
}

//PROTECTION
ActionProtectedAgainstDamage* ActionCreationFactory::createActionProtectedAgainstDamage()
{
    return new ActionProtectedAgainstDamage();
}

ActionProtectedAgainstDamageRandom* ActionCreationFactory::createActionProtectedAgainstDamageRandom()
{
    return new ActionProtectedAgainstDamageRandom();
}

ActionCompleteProtectionByPayingOneEnergy* ActionCreationFactory::createActionCompleteProtectionByPayingOneEnergy(AbstractCard::Enum_element elementToRemove)
{
    return new ActionCompleteProtectionByPayingOneEnergy(elementToRemove);
}

ActionNoDamageOnThreshold* ActionCreationFactory::createActionNoDamageOnThreshold(unsigned short damage)
{
    return new ActionNoDamageOnThreshold(damage);
}

ActionCompleteProtectionRandom* ActionCreationFactory::createActionCompleteProtectionRandom()
{
    return new ActionCompleteProtectionRandom();
}

//STATUS
ActionChangeEnemyStatus* ActionCreationFactory::createActionChangeEnemyStatus(CardPokemon::Enum_statusOfPokemon status)
{
    return new ActionChangeEnemyStatus(status);
}

ActionChangeEnemyStatusRandom* ActionCreationFactory::createActionChangeEnemyStatusRandom(CardPokemon::Enum_statusOfPokemon statusOnHead, CardPokemon::Enum_statusOfPokemon statusOnTail)
{
    return new ActionChangeEnemyStatusRandom(statusOnHead, statusOnTail);
}

ActionEnemyPoisoned* ActionCreationFactory::createActionEnemyPoisoned(unsigned short damage)
{
    return new ActionEnemyPoisoned(damage);
}

//BANC
ActionHurtEveryPokemonOnOwnBench* ActionCreationFactory::createActionHurtEveryPokemonOnOwnBench(unsigned short damage)
{
    return new ActionHurtEveryPokemonOnOwnBench(damage);
}

ActionDieAndHurtEveryPokemonOnEachBench* ActionCreationFactory::createActionDieAndHurtEveryPokemonOnEachBench(unsigned short damage)
{
    return new ActionDieAndHurtEveryPokemonOnEachBench(damage);
}

ActionSwapPokemonBetweenFigthAndBench* ActionCreationFactory::createActionSwapPokemonBetweenFigthAndBench()
{
    return new ActionSwapPokemonBetweenFigthAndBench();
}


//ACTIONS TRAINERS
ActionTrainer_InformaticSearch* ActionCreationFactory::createActionTrainer_InformaticSearch()
{
    return new ActionTrainer_InformaticSearch();
}

ActionTrainer_FakeProfessorChen* ActionCreationFactory::createActionTrainer_FakeProfessorChen()
{
    return new ActionTrainer_FakeProfessorChen();
}

ActionTrainer_Restored* ActionCreationFactory::createActionTrainer_Restored()
{
    return new ActionTrainer_Restored();
}

ActionTrainer_SaveEnergies* ActionCreationFactory::createActionTrainer_SaveEnergies()
{
    return new ActionTrainer_SaveEnergies();
}

ActionTrainer_TotalGuerison* ActionCreationFactory::createActionTrainer_TotalGuerison()
{
    return new ActionTrainer_TotalGuerison();
}

ActionTrainer_PokemonCenter* ActionCreationFactory::createActionTrainer_PokemonCenter()
{
    return new ActionTrainer_PokemonCenter();
}

ActionTrainer_ProfessorChen* ActionCreationFactory::createActionTrainer_ProfessorChen()
{
    return new ActionTrainer_ProfessorChen();
}

ActionTrainer_SuperPotion* ActionCreationFactory::createActionTrainer_SuperPotion()
{
    return new ActionTrainer_SuperPotion();
}

ActionTrainer_Leo* ActionCreationFactory::createActionTrainer_Leo()
{
    return new ActionTrainer_Leo();
}

ActionTrainer_RemoveEnergy* ActionCreationFactory::createActionTrainer_RemoveEnergy()
{
    return new ActionTrainer_RemoveEnergy();
}

ActionTrainer_Potion* ActionCreationFactory::createActionTrainer_Potion()
{
    return new ActionTrainer_Potion();
}*/
