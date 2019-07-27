#ifndef ABSTRACTACTION_H
#define ABSTRACTACTION_H

#include <QDebug>
#include <QObject>

#include "gamemanager.h"
#include "player.h"
#include "src_Cards/cardpokemon.h"
#include "src_Packets/bencharea.h"
#include "src_Packets/fightarea.h"

#include "src_Log/log.h"

class AbstractAction
{
public:
	enum Enum_typeOfAction
	{
        //GENERAL
        Action_None = 0,
        Action_ChangeWeaknessOfEnemy = 1,
        Action_ChangeResistanceOfHimself = 2,
        Action_EnemyDieIfYouDieInNextTurn = 3,

        //ATTAQUES
        Action_ReplicateOneAttackFromEnemy = 100,
        Action_ReplicateLastDamageToEnemy = 101,
        Action_UniqueAttack_Random = 102,                   //Index of attack
        Action_AttackOnlyIfEnemyIsSleeping = 103,           //Quantity of damage
        Action_BlockOneEnemyAttackForOneTurn = 104,
        //Action_CanAttack_Random = 105,                      //Quantity of damage
        Action_EnemyCanAttackOnNextTurn_Random = 106,

        //GESTION DES DEGATS
        Action_MoreDamageByEnergy = 200,                    //Quantity of damage per energy - index of attack
        Action_DamageMultipliedByHeadOrTail = 201,          //Quantity of damage - number of coins
        Action_MoreDamageByEnemyDamage = 202,               //Quantity of damage per marquer
        Action_MoreDamageByOwnDamage = 203,                 //Quantity of damage
        Action_AttackLessDamageOnHimself = 204,             //Original damage - less damage per marquer
        Action_DamageOfHalfEnemyLifeLeft = 205,
        Action_HurtHimself = 206,                           //Quantity of damage
        Action_MoreDamageOrHurtHimSelf  = 207,              //Quantity of damage on head - quantity of damage on tail
        Action_MoreDamageByEnergyOnEnemy = 208,             //Quantity of damage per energy

        //ENERGIES
        Action_RemoveEnergyAttached = 300,                  //Number of energy - type of energy
        Action_RemoveOneEnergyOnEnemy = 301,                //Number of energy - Type of energy

        //SOIN
        Action_Healing = 400,                               //Quantity of heal - type of energy

        //PROTECTION
        Action_ProtectedAgainstDamage = 500,
        Action_ProtectedAgainstDamageRandom = 501,
        Action_CompleteProtectionByPayingOneEnergy = 502,	//Type of energy
        Action_NoDamageOnThreshold = 503,                   //Limit of damage
        Action_CompleteProtectionRandom = 504,

        //STATUS
        Action_ChangeEnemyStatus = 600,                     //Status to apply
        Action_ChangeEnemyStatus_Random = 601,              //Status to apply on head - Status to apply on tail
        Action_EnemyPoisoned = 602,                         //Quantity of damage per round

        //BANC
        Action_HurtEveryPokemonOnOwnBench = 700,            //Quantity of damage
        Action_DieAndHurtEveryPokemonOnEachBench = 701,     //Quantity of damage for each pokemon in Bench
        Action_SwapPokemonBetweenFigthAndBench = 702,

        //CARTES DRESSEUR
        ActionTrainer_InformaticSearch = 2001,
        ActionTrainer_FakeProfessorChen = 2003,
        ActionTrainer_Restored = 2004,
        ActionTrainer_LittleGirl = 2005,
        ActionTrainer_SaveEnergies = 2011,
        ActionTrainer_TotalGuerison = 2012,
        ActionTrainer_PokemonCenter = 2015,
        ActionTrainer_ProfessorChen = 2018,
        ActionTrainer_SuperPotion = 2020,
        ActionTrainer_Leo = 2021,
        ActionTrainer_RemoveEnergy = 2022,
        ActionTrainer_Potion = 2024
    };

    enum Enum_ElementsToCheck
    {
        CheckGameManager = 0,
        CheckPlayerAttacked,
        CheckPlayerAttacking,
        CheckPokemonAttached,
        CheckPokemonAttacked,
        CheckPokemonAttacking,
        CheckBenchPlayerAttacked,
        CheckBenchPlayerAttacking
    };

    AbstractAction();
    AbstractAction(QVariant arg1);
    AbstractAction(QVariant arg1, QVariant arg2);
    virtual ~AbstractAction();

    QVariant arg1();
    QVariant arg2();
	
    virtual Enum_typeOfAction type() = 0;
    virtual bool checkElements() { return true; }
    void executeActionBeforeAttack(CardPokemon* pokemonAttached = nullptr, short index = -1);
    void executeActionAfterAttack(CardPokemon* pokemonAttached = nullptr, short index = -1);

#ifdef TESTS_UNITAIRES
    QList<unsigned short> listCoins();
#endif


protected:
    QVariant m_arg1, m_arg2;

    virtual QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() = 0;
    virtual void actionBeforeAttack();
    virtual void actionAfterAttack();

    unsigned short headOrTail();

    GameManager* gameManager();
    Player* playerAttacked();
    Player* playerAttacking();
    CardPokemon* pokemonAttached();
    CardPokemon* pokemonAttacked();
    CardPokemon* pokemonAttacking();
    BenchArea* benchPlayerAttacked();
    BenchArea* benchPlayerAttacking();
    short indexAttack();

private:
    GameManager* m_gameManager;
    Player* m_playerAttacked;
    Player* m_playerAttacking;
    CardPokemon* m_pokemonAttached;
    CardPokemon* m_pokemonAttacked;
    CardPokemon* m_pokemonAttacking;
    BenchArea* m_benchPlayerAttacked;
    BenchArea* m_benchPlayerAttacking;
    short m_indexAttack;

#ifdef TESTS_UNITAIRES
    QList<unsigned short> m_listCoins;
#endif

    bool checkGameManager();
    bool checkPlayerAttacked();
    bool checkPlayerAttacking();
    bool checkPokemonAttached();
    bool checkPokemonAttacked();
    bool checkPokemonAttacking();
    bool checkBenchPlayerAttacked();
    bool checkBenchPlayerAttacking();
};

#endif // ABSTRACTACTION_H
