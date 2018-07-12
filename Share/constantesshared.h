#ifndef CONSTANTESSHARED_H
#define CONSTANTESSHARED_H

#include <QObject>

class ConstantesShared
{
public:
    enum GamePhase
    {
        PHASE_Identification = 1,
        PHASE_ListOfGameForThePlayer = 2,
        PHASE_GetAllInfoOnInstance = 3,
        PHASE_ListOfAllPlayers = 10,
        PHASE_CreateANewGame = 11,
        PHASE_RemoveAGame = 12,
        PHASE_GetAllInfoOnGame = 20,
        PHASE_SelectCards = 31,
        PHASE_InitReady = 32,
        PHASE_MoveACard = 40,
        PHASE_Attack_Retreat = 41,
        PHASE_SkipTheTurn = 42,
        PHASE_NotifCardMoved = 100,
        PHASE_NotifPlayerIsReady = 101,
        PHASE_NotifPokemonAttack = 102
    };

    ConstantesShared();
};

#endif // CONSTANTESSHARED_H
