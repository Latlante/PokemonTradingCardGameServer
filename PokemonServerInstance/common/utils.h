#ifndef UTILS_H
#define UTILS_H

#include <QObject>

#define MAXCARDS_BENCH      5
#define MAXCARDS_FIGHT      1
#define MAXCARDS_DECK       60
#define MAXCARDS_HAND       -1
#define MAXCARDS_REWARD     6
#define MAXCARDS_TRASH      60

#define NAME_BENCH          "Bench"
#define NAME_DECK           "Deck"
#define NAME_FIGHT          "Fight"
#define NAME_HAND           "Hand"
#define NAME_REWARDS        "Rewards
#define NAME_TRASH          "Trash"

#define DAMAGE_POISON           10
#define DAMAGE_WHEN_CONFUSED    30

#define DAMAGE_MARQUER_VALUE    10

#define MAX_USHORT              0xFFFF

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = nullptr);

    static void initRandomValues();
    static unsigned short headOrTail();
    static unsigned short selectFirstPlayer(int count);
    static unsigned short selectRandomCardsPokemon();
    static unsigned short selectRandomCardsEnergy();

//private:
    static int randomValue(int min, int max);
};

#endif // UTILS_H
