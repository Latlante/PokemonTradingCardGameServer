#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "player.h"

class GameManager;
class Log;
class StdListenerWritter;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(const QString& nameGame,
                        const QString& player1,
                        const QString& player2,
                        QObject *parent = nullptr);

signals:

private slots:
    void onMessageReceived_Communication(QByteArray message);

private:
    StdListenerWritter* m_communication;
    GameManager* m_gameManager;
    Log m_log;

    QJsonObject selectCardPerPlayer(const QString& namePlayer, QJsonArray tabCards);
    QJsonObject moveACard(const QString& namePlayer, Player::EnumPacket packetOrigin, Player::EnumPacket packetDestination, int indexCardOrigin, int indexCardDestination);
};

#endif // CONTROLLER_H
