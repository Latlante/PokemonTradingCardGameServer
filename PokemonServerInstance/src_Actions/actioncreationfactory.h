#ifndef ACTIONCREATIONFACTORY_H
#define ACTIONCREATIONFACTORY_H

#include "src_Actions/abstractaction.h"
#include "src_Cards/cardpokemon.h"

//#define CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(TYPE)   static TYPE* create##TYPE() { return new TYPE(); }
//#define CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(TYPE)   static TYPE* create##TYPE(QVariant arg) { return new TYPE(arg); }
//#define CREATE_ACTION_FROM_ENUM_WITH_TWO_ARG(TYPE)   static TYPE* create##TYPE(QVariant arg1, QVariant arg2) { return new TYPE(arg1, arg2); }

class ActionCreationFactory
{
public:
    ActionCreationFactory();

    static AbstractAction* createAction(AbstractAction::Enum_typeOfAction idAction, QVariant arg1 = QVariant(), QVariant arg2 = QVariant());

    //GENERAL
    /*CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionChangeWeaknessOfEnemy)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionChangeResistanceOfHimself)
    //CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionEnemyDieIfYouDieInNextTurn)

    //ATTAQUES
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionReplicateOneAttackFromEnemy)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionReplicateLastDamageToEnemy)
    CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(ActionUniqueAttack_Random)
    CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(ActionAttackOnlyIfEnemyIsSleeping)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionBlockOneEnemyAttackForOneTurn)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionEnemyCanAttackOnNextTurn_Random)

    //GESTION DES DEGATS
    CREATE_ACTION_FROM_ENUM_WITH_TWO_ARG(ActionMoreDamageByEnergy)
    CREATE_ACTION_FROM_ENUM_WITH_TWO_ARG(ActionDamageMultipliedByHeadOrTail)
    CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(ActionMoreDamageByEnemyDamage)
    CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(ActionMoreDamageByOwnDamage)
    CREATE_ACTION_FROM_ENUM_WITH_TWO_ARG(ActionAttackLessDamageOnHimself)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionDamageOfHalfEnemyLifeLeft)
    CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(ActionHurtHimself)
    CREATE_ACTION_FROM_ENUM_WITH_TWO_ARG(ActionMoreDamageOrHurtHimSelf)
    CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(ActionMoreDamageByEnergyOnEnemy)

    //ENERGIES
    CREATE_ACTION_FROM_ENUM_WITH_TWO_ARG(ActionRemoveEnergyAttached)
    CREATE_ACTION_FROM_ENUM_WITH_TWO_ARG(ActionRemoveOneEnergyOnEnemy)

    //SOIN
    CREATE_ACTION_FROM_ENUM_WITH_TWO_ARG(ActionHealing)

    //PROTECTION
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionProtectedAgainstDamage)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionProtectedAgainstDamageRandom)
    CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(ActionCompleteProtectionByPayingOneEnergy)
    CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(ActionNoDamageOnThreshold)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionCompleteProtectionRandom)

    //STATUS
    CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(ActionChangeEnemyStatus)
    CREATE_ACTION_FROM_ENUM_WITH_TWO_ARG(ActionChangeEnemyStatus_Random)
    CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(ActionEnemyPoisoned)

    //BANC
    CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(ActionHurtEveryPokemonOnOwnBench)
    CREATE_ACTION_FROM_ENUM_WITH_ONE_ARG(ActionHurtEveryPokemonOnOwnBench)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionSwapPokemonBetweenFigthAndBench)

    //ACTIONS TRAINERS
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionTrainer_InformaticSearch)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionTrainer_FakeProfessorChen)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionTrainer_Restored)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionTrainer_LittleGirl)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionTrainer_SaveEnergies)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionTrainer_TotalGuerison)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionTrainer_PokemonCenter)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionTrainer_ProfessorChen)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionTrainer_SuperPotion)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionTrainer_Leo)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionTrainer_RemoveEnergy)
    CREATE_ACTION_FROM_ENUM_WITH_NO_ARG(ActionTrainer_Potion)*/


    //GENERAL
    /*static ActionChangeWeaknessOfEnemy* createActionChangeWeaknessOfEnemy(AbstractCard::Enum_element element);
    static ActionChangeResistanceOfHimself* createActionChangeResistanceOfHimself(AbstractCard::Enum_element element);

    //ATTAQUES
    static ActionReplicateOneAttackFromEnemy* createActionReplicateOneAttackFromEnemy();
    static ActionReplicateLastDamageToEnemy* createActionReplicateLastDamageToEnemy();
    static ActionUniqueAttack_Random* createActionUniqueAttack_Random(unsigned short index);
    static ActionAttackOnlyIfEnemyIsSleeping* createActionAttackOnlyIfEnemyIsSleeping(unsigned short damage);
    static ActionBlockOneEnemyAttackForOneTurn* createActionBlockOneEnemyAttackForOneTurn();
    static ActionEnemyCanAttackOnNextTurn_Random* createActionEnemyCanAttackOnNextTurn_Random();


    //GESTION DES DEGATS
    static ActionMoreDamageByEnergy* createActionMoreDamageByEnergy(unsigned short damagePerEnergy, unsigned short indexOfAttack);
    static ActionDamageMultipliedByHeadOrTail* createActionDamageMultipliedByHeadOrTail(unsigned short damagePerHead, unsigned short numberOfCoins);
    static ActionMoreDamageByEnemyDamage* createActionMoreDamageByEnemyDamage(unsigned short damagePerMarquer);
    static ActionMoreDamageByOwnDamage* createActionMoreDamageByOwnDamage(unsigned short damage);
    static ActionAttackLessDamageOnHimself* createActionAttackLessDamageOnHimself(unsigned short originalDamage, unsigned short damagePerMarquer);
    static ActionDamageOfHalfEnemyLifeLeft* createActionDamageOfHalfEnemyLifeLeft();
    static ActionHurtHimself* createActionHurtHimself(unsigned short damage);
    static ActionMoreDamageOrHurtHimSelf* createActionMoreDamageOrHurtHimSelf(unsigned short damageOnHead, unsigned short damageOnTail);
    static ActionMoreDamageByEnergyOnEnemy* createActionMoreDamageByEnergyOnEnemy(unsigned short damagePerEnergy);

    //ENERGIES
    static ActionRemoveEnergyAttached* createActionRemoveEnergyAttached(unsigned short numberOfEnergies, AbstractCard::Enum_element elementToRemove);
    static ActionRemoveOneEnergyOnEnemy* createActionRemoveOneEnergyOnEnemy(unsigned short numberOfEnergies, AbstractCard::Enum_element elementToRemove);

    //SOIN
    static ActionHealing* createActionHealing(unsigned short pv, AbstractCard::Enum_element energyToPay);

    //PROTECTION
    static ActionProtectedAgainstDamage* createActionProtectedAgainstDamage();
    static ActionProtectedAgainstDamageRandom* createActionProtectedAgainstDamageRandom();
    static ActionCompleteProtectionByPayingOneEnergy* createActionCompleteProtectionByPayingOneEnergy(AbstractCard::Enum_element elementToRemove);
    static ActionNoDamageOnThreshold* createActionNoDamageOnThreshold(unsigned short damage);
    static ActionCompleteProtectionRandom* createActionCompleteProtectionRandom();

    //STATUS
    static ActionChangeEnemyStatus* createActionChangeEnemyStatus(CardPokemon::Enum_statusOfPokemon status);
    static ActionChangeEnemyStatusRandom* createActionChangeEnemyStatusRandom(CardPokemon::Enum_statusOfPokemon statusOnHead, CardPokemon::Enum_statusOfPokemon statusOnTail);
    static ActionEnemyPoisoned* createActionEnemyPoisoned(unsigned short damage);

    //BANC
    static ActionHurtEveryPokemonOnOwnBench* createActionHurtEveryPokemonOnOwnBench(unsigned short damage);
    static ActionDieAndHurtEveryPokemonOnEachBench* createActionDieAndHurtEveryPokemonOnEachBench(unsigned short damage);
    static ActionSwapPokemonBetweenFigthAndBench* createActionSwapPokemonBetweenFigthAndBench();

    //ACTIONS TRAINERS
    static ActionTrainer_InformaticSearch* createActionTrainer_InformaticSearch();
    static ActionTrainer_FakeProfessorChen* createActionTrainer_FakeProfessorChen();
    static ActionTrainer_Restored* createActionTrainer_Restored();
    static ActionTrainer_SaveEnergies* createActionTrainer_SaveEnergies();
    static ActionTrainer_TotalGuerison* createActionTrainer_TotalGuerison();
    static ActionTrainer_PokemonCenter* createActionTrainer_PokemonCenter();
    static ActionTrainer_ProfessorChen* createActionTrainer_ProfessorChen();
    static ActionTrainer_SuperPotion* createActionTrainer_SuperPotion();
    static ActionTrainer_Leo* createActionTrainer_Leo();
    static ActionTrainer_RemoveEnergy* createActionTrainer_RemoveEnergy();
    static ActionTrainer_Potion* createActionTrainer_Potion();*/


};

#endif // ACTIONCREATIONFACTORY_H
