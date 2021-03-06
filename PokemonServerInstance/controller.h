#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "player.h"
#include "../Share/constantesshared.h"
#include "src_Cards/abstractcard.h"
#include "src_Log/historicalnotifications.h"
#include "src_Log/log.h"

class AbstractDisplayData;
class GameManager;
class SocketToServer;
class LocalSocketToServer;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(const QString& uidGame,
                        const QString& nameGame,
                        const QString& player1,
                        const QString& player2,
                        QObject *parent = nullptr);
    ~Controller();

signals:
    void selectionDisplayFinished();

private slots:
    void onMessageReceived_Communication(QByteArray message);
    void onLogReceived(QString message);

    void onInitReadyChanged_GameManager();
    void onCardMoved_GameManager(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, int idCard, bool showCardToEveryone);
    void onIndexCurrentPlayerChanged_GameManager(const QString& oldPlayer, const QString& newPlayer);
    void onDataPokemonChanged_GameManager(const QString& namePlayer, ConstantesShared::EnumPacket packet, int indexCard, CardPokemon* pokemon);
    void onPokemonSwitched_GameManager(const QString& namePlayer, ConstantesShared::EnumPacket packet, int indexCard, int newIdCard, bool keepEnergy);
    void onEnergyAdded_GameManager(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, int idEnergy);
    void onEnergyRemoved_GameManager(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, int indexEnergy);
    void onHeadOrTailDone_GameManager(const QString& namePlayer, QList<unsigned short> coins);
    void onEndOfGame_GameManager(const QString& nameWinner, const QString& nameLoser);

    void onDisplayPacketAsked(const QString &namePlayer, AbstractPacket *packet, unsigned short quantity, AbstractCard::Enum_typeOfCard typeOfCard);
    void onDisplayAllElementsAsked(const QString &namePlayer, unsigned short quantity);
    void onDisplayHiddenPacketAsked(const QString &namePlayer, AbstractPacket *packet, unsigned short quantity);
    void onDisplayEnergiesForAPokemonAsked(const QString &namePlayer, CardPokemon* pokemon, unsigned short quantity, AbstractCard::Enum_element element);
    void onDisplayAttacksAsked(const QString& namePlayer, CardPokemon* pokemon, bool retreatEnable);

private:
    LocalSocketToServer* m_communication;
    GameManager* m_gameManager;
    HistoricalNotifications m_historicNotif;
    AbstractDisplayData *m_displayData;

    QJsonObject getAllInfoOnTheGame(const QString& namePlayer);
    QJsonObject selectCardPerPlayer(const QString& namePlayer, QJsonArray tabCards);
    QJsonObject moveACard(const QString& namePlayer, Player::EnumPacket packetOrigin, Player::EnumPacket packetDestination, int indexCardOrigin, int indexCardDestination);
    QJsonObject setInitReadyForAPlayer(const QString& namePlayer);
    QJsonObject attack_retreat(const QString& namePlayer, unsigned short indexAttack);
    QJsonObject skipTurn(const QString& namePlayer);

    QJsonObject displayPacketResponse(const QJsonDocument &document);

    void sendNotifNewGameCreated(int uid, const QString& nameGame, const QString &player1, const QString &player2);
    void sendNotifPlayerIsReady();
    void sendNotifEndOfTurn(const QString& oldPlayer, const QString& newPlayer);
    void sendNotifCardMoved(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, int idCard, bool showCardToEveryone);
    void sendNotifDataPokemonChanged(const QString& namePlayer, ConstantesShared::EnumPacket packet, int indexCard, CardPokemon* pokemon);
    void sendNotifPokemonSwitched(const QString& namePlayer, ConstantesShared::EnumPacket packet, int indexCard, int newIdCard, bool keepEnergy);
    void sendNotifEnergyAdded(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, int idEnergy);
    void sendNotifEnergyRemoved(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, int indexEnergy);
    void sendNotifHeadOrTailDone(const QString& namePlayer, QList<unsigned short> coins);
    void sendNotifEndOfGame(const QString& nameWinner, const QString& nameLoser);

    void sendNotifDisplayPacket(const QString &namePlayer, QMap<int,int> cards);

    QString nameOfEnemy(const QString& nameCurrentPlayer);
};

#endif // CONTROLLER_H
