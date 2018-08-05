#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include "player.h"

#include "common/constantesqml.h"
#include "src_Cards/abstractcard.h"
#include "src_Cards/cardpokemon.h"
#include "../Share/constantesshared.h"

class CtrlPopups;

class GameManager : public QObject
{
	Q_OBJECT
	
public:
    GameManager(QObject *parent = NULL);
	~GameManager();

    static GameManager* createInstance();
    static void deleteInstance();
    static GameManager* instance();
	
    bool moveACard(const QString& namePlayer, Player::EnumPacket packetOrigin, Player::EnumPacket packetDestination, unsigned int indexCardOrigin = 0, unsigned int indexCardDestination = 0);

    //Accesseurs
    QList<Player*> listOfPlayers();
    Player* currentPlayer();
    Player* playerAttacked();
    Player* playerAt(int index);
    Player* playerByName(const QString& name);
    Player* enemyOf(Player* play);
    ConstantesQML::StepGame gameStatus();
    void setGameStatus(ConstantesQML::StepGame step);

    //Preparation de la partie
    bool initGame();
    unsigned short numberMaxOfPlayers();
    void setNumberMaxOfPlayers(unsigned short max);
    Player* addNewPlayer(QString name, QList<AbstractCard*> listCards);
    void removePlayer(Player* play);
    bool initPlayer(Player* play);
    void selectFirstPlayer();
    void setInitReady(Player* playerReady);

    //Phase de combat
    void startGame();
	void nextPlayer();
    CardPokemon::Enum_StatusOfAttack attack(CardPokemon* pokemonAttacking, unsigned short index);
    bool retreat(CardPokemon *pokemonToRetreat);
	void endOfTurn();

    //Fin de la game
    Player *gameIsFinished();

    QList<AbstractCard *> displayPacket(AbstractPacket *packet, unsigned short quantity = 1, AbstractCard::Enum_typeOfCard typeOfCard = AbstractCard::TypeOfCard_Whatever);
    AbstractCard::Enum_element displayAllElements(unsigned short quantity = 1);
    QList<AbstractCard *> displaySelectHiddenCard(PacketRewards *packet, unsigned short quantity = 1);
    QList<CardEnergy *> displayEnergiesForAPokemon(CardPokemon* pokemon, unsigned short quantity, AbstractCard::Enum_element element);
    int displayAttacks(CardPokemon* card, bool blockRetreat = false);
    void displayMessage(QString message);
    void endOfSelectionDisplay(QVariant element = QVariant());

#ifdef TESTS_UNITAIRES
    void setForcedValueHeadOrTail(bool forced, unsigned short value = 0);
#endif
    unsigned short headOrTail();


signals:
    void indexCurrentPlayerChanged(const QString&,const QString&);
    void gameStatusChanged();
    void replacePokemonFighter(Player*);

    void displayPacketAsked(AbstractPacket*, unsigned short, AbstractCard::Enum_typeOfCard);
    void displayAllElementsAsked(unsigned short);
    void displaySelectHiddenCardAsked(PacketRewards *, unsigned short);
    void displayEnergiesForAPokemonAsked(CardPokemon*, unsigned short, AbstractCard::Enum_element);
    void displayAttacksAsked(CardPokemon*, bool);
    void displayMessageAsked(QString);
    void headOrTailAsked();
    void selectionDisplayFinished();

    void logReceived(QString);
    void movingCardAnimationStartAsked();

    void initReadyChanged();
    void cardMoved(const QString&,ConstantesShared::EnumPacket,int,ConstantesShared::EnumPacket,int,bool);
    void dataPokemonChanged(const QString&,ConstantesShared::EnumPacket,int,CardPokemon*);
    void energyAdded(const QString&, ConstantesShared::EnumPacket, int, int);
    void energyRemoved(const QString&, ConstantesShared::EnumPacket, int, int);

private slots:
    void onEndOfTurn_Player();

private:
#ifdef TESTS_UNITAIRES
    bool m_forcedValueHeadOrTail;
    unsigned short m_nextValueHeadOrTail;
#else

#endif

    static const int m_NUMBER_FIRST_CARDS;
    static const int m_NUMBER_REWARDS;
    static const int m_NUMBER_MAX_TRY_CHECK_HAND;
    static GameManager *m_instance;
    unsigned short m_numberMaxOfPlayers;
	QList<Player*> m_listPlayers;
    short m_indexCurrentPlayer;
    Player* m_playerAttacking;
    Player* m_playerAttacked;
    ConstantesQML::StepGame m_gameStatus;
	
	bool m_gameIsReady;
    //QList<AbstractCard*> m_listSelectionCards;
    QVariant m_elementFromDisplays;

    void setIndexCurrentPlayer(int index);

    bool checkHandOfEachPlayer();
    void drawFirstCards(Player *play);
    void checkPokemonDead();
    void checkStatusPokemonForNewRound();
    void checkAttacksBlocked();


};

#endif // GAMEMANAGER_H
