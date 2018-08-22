#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "player.h"
#include "../Share/constantesshared.h"
#include "src_Cards/abstractcard.h"
#include "src_Log/historicalnotifications.h"
#include "src_Log/log.h"

class GameManager;
class SocketToServer;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(const QString& uidGame,
                        const QString& nameGame,
                        const QString& uidPlayer1,
                        const QString& namePlayer1,
                        const QString& uidPlayer2,
                        const QString& namePlayer2,
                        QObject *parent = nullptr);
    ~Controller();

signals:

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

    void onDisplayPacketAsked(AbstractPacket *packet, unsigned short quantity, AbstractCard::Enum_typeOfCard typeOfCard);

private:
    SocketToServer* m_communication;
    GameManager* m_gameManager;
    HistoricalNotifications m_historicNotif;
    //Log m_log;

    QJsonObject getAllInfoOnTheGame(const unsigned int uidPlayer);
    QJsonObject selectCardPerPlayer(const unsigned int uidPlayer, QJsonArray tabCards);
    QJsonObject moveACard(const unsigned int uidPlayer, Player::EnumPacket packetOrigin, Player::EnumPacket packetDestination, int indexCardOrigin, int indexCardDestination);
    QJsonObject setInitReadyForAPlayer(const unsigned int uidPlayer);
    QJsonObject attack_retreat(const unsigned int uidPlayer, unsigned short indexAttack);
    QJsonObject skipTurn(const unsigned int uidPlayer);

    void sendNotifPlayerIsReady();
    void sendNotifEndOfTurn(const QString& oldPlayer, const QString& newPlayer);
    void sendNotifCardMoved(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, int idCard, bool showCardToEveryone);
    void sendNotifDataPokemonChanged(const QString& namePlayer, ConstantesShared::EnumPacket packet, int indexCard, CardPokemon* pokemon);
    void sendNotifPokemonSwitched(const QString& namePlayer, ConstantesShared::EnumPacket packet, int indexCard, int newIdCard, bool keepEnergy);
    void sendNotifEnergyAdded(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, int idEnergy);
    void sendNotifEnergyRemoved(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, int indexEnergy);

    void sendNotifDisplayPacket(const QString &namePlayer, QMap<int,int> cards);
};

#endif // CONTROLLER_H
