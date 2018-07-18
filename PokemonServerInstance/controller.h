#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "player.h"
#include "../Share/constantesshared.h"
#include "src_Log/historicalnotifications.h"
#include "src_Log/log.h"

class GameManager;
class StdListenerWritter;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(const QString& nameGame,
                        const QString& player1,
                        const QString& player2,
                        QObject *parent = nullptr);
    ~Controller();

signals:

private slots:
    void onMessageReceived_Communication(QString message);
    void onLogReceived(QString message);

    void onIndexCurrentPlayerChanged_GameManager(const QString& oldPlayer, const QString& newPlayer);

private:
    StdListenerWritter* m_communication;
    GameManager* m_gameManager;
    HistoricalNotifications m_historicNotif;
    Log m_log;

    QJsonObject selectCardPerPlayer(const QString& namePlayer, QJsonArray tabCards);
    QJsonObject moveACard(const QString& namePlayer, Player::EnumPacket packetOrigin, Player::EnumPacket packetDestination, int indexCardOrigin, int indexCardDestination);
    QJsonObject setInitReadyForAPlayer(const QString& namePlayer);
    QJsonObject attack_retreat(const QString& namePlayer, unsigned short indexAttack);
    QJsonObject skipTurn(const QString& namePlayer);

    void sendNotifPlayerIsReady();
    void sendNotifEndOfTurn(const QString& oldPlayer, const QString& newPlayer);
    void sendNotifCardMoved(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, int indexCardDestination, int idCard);
    void sendNotifDataPokemonChanged(const QString& namePlayer, ConstantesShared::EnumPacket packet, int indexCard, CardPokemon* pokemon);
};

#endif // CONTROLLER_H
