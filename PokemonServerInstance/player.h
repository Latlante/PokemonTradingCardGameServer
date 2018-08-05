#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QVariant>
#include "../Share/constantesshared.h"

class AbstractCard;
class CardAction;
class CardEnergy;
class CardPokemon;
class AbstractPacket;
class PacketDeck;
class PacketRewards;
class PacketHand;
class BenchArea;
class FightArea;
class PacketTrash;

class Player : public QObject
{
	Q_OBJECT
    Q_PROPERTY(bool initReady READ initReady WRITE setInitReady NOTIFY initReadyChanged)
    Q_PROPERTY(bool canPlay READ canPlay WRITE setCanPlay NOTIFY canPlayChanged)
	
public:
    enum EnumPacket
    {
        PACKET_None = 0,
        PCK_Bench,
        PCK_Deck,
        PCK_Fight,
        PCK_Hand,
        PCK_Rewards,
        PCK_Trash
    };

	Player(QString name, QList<AbstractCard*> listCards, QObject *parent = NULL);
	~Player();
	
    const QString name();
    BenchArea* bench();
    PacketDeck* deck();
    FightArea* fight();
    PacketHand* hand();
    PacketRewards* rewards();
    PacketTrash* trash();
	
	void newTurn();
    void turnFinished();
    bool isPlaying();
	void drawOneCard();
    void drawOneReward(AbstractCard *cardReward);
    bool isLoser();

    bool initReady();
    void setInitReady(bool ready);
    bool setInitReadyIfReady();

    bool canPlay();
    void setCanPlay(bool status);

    bool moveCardFromDeckToHand(AbstractCard* cardDeckToMove = nullptr);
    bool moveCardFromDeckToReward(AbstractCard* cardDeckToMove = nullptr);
    bool moveCardFromHandToBench(int indexHand, int indexBench);
    bool moveCardFromHandToDeck(AbstractCard* cardHandToMove);
    bool moveCardFromHandToFight(int indexHand);
    bool moveCardFromHandToTrash(AbstractCard *cardHandToMove);
    bool moveCardFromBenchToFight(CardPokemon *pokemonToMove);
    bool moveCardFromBenchToTrash(int index);
    bool moveCardFromFightToTrash(int index);
    bool moveCardFromRewardToHand(AbstractCard *cardReward);
    bool moveCardFromTrashToHand(AbstractCard *cardTrash);
    bool moveAllCardFromHandToDeck();

    bool swapCardsBetweenBenchAndFight(CardPokemon *pokemonBenchToSwap);


signals:
    void endOfTurn();
    void canPlayChanged();
    void initReadyChanged();

    void cardMoved(const QString&,ConstantesShared::EnumPacket,int,ConstantesShared::EnumPacket,int,bool);
    void dataPokemonChanged(const QString&,ConstantesShared::EnumPacket,int,CardPokemon*);
    void energyAdded(const QString&, ConstantesShared::EnumPacket, int, int);
    void energyRemoved(const QString&, ConstantesShared::EnumPacket, int, int);

private slots:
    void onDataChanged_CardPokemon();
    void onEnergyAdded_CardPokemon(int idEnergy);
    void onEnergyRemoved_CardPokemon(int indexEnergy);

private:
	QString m_name;

    BenchArea* m_bench;
	PacketDeck* m_deck;
    FightArea* m_fight;
    PacketHand* m_hand;
	PacketRewards* m_rewards;
    PacketTrash* m_trash;

    //For the round
    bool m_initReady;
    bool m_canPlay;
    bool m_energyPlayedForThisRound;

    bool moveCardFromPacketToAnother(AbstractPacket* source, AbstractPacket* destination, int index);
    bool moveCardFromPacketToAnother(AbstractPacket* source, AbstractPacket* destination, AbstractCard* cardToMove);

};

#endif // PLAYER_H
