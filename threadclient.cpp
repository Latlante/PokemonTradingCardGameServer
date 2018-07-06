#include "threadclient.h"

#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QTcpSocket>
#include <QVariant>

#include "authentification.h"
#include "instancemanager.h"

ThreadClient::ThreadClient(int socketDescriptor, QObject *parent) :
    QThread(parent),
    m_socketDescriptor(socketDescriptor),
    m_tcpSocket(nullptr),
    m_user(""),
    m_uid(-1),
    m_token("")
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void ThreadClient::run()
{
    qDebug() << "Starting thread: " << m_socketDescriptor;
    m_tcpSocket = new QTcpSocket();

    if(!m_tcpSocket->setSocketDescriptor(m_socketDescriptor))
    {
        qCritical() << "Error creation thread:" << m_tcpSocket->errorString();
        return;
    }

    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &ThreadClient::onReadyRead_TcpSocket, Qt::DirectConnection);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &ThreadClient::onDisconnected_TcpSocket, Qt::DirectConnection);

    exec();
}

void ThreadClient::onReadyRead_TcpSocket()
{
    QByteArray message = m_tcpSocket->readAll();

    qDebug() << __PRETTY_FUNCTION__ << m_socketDescriptor << ", " << message;

    QJsonDocument jsonReceived = QJsonDocument::fromJson(message);

    if(!jsonReceived.isEmpty())
    {
        QJsonObject jsonResponse;

        //if no authentify yet
        if(m_uid == -1)
        {
            jsonResponse = authentify(jsonReceived["name"].toString(),
                                      jsonReceived["password"].toString());
        }
        //authentification success
        else
        {
            QString tokenReceive = jsonReceived["token"].toString();

            //everything is good
            if(tokenReceive == m_token)
            {
                int phase = jsonReceived["phase"].toInt();
                qDebug() << __PRETTY_FUNCTION__ << "phase:" << phase;

                switch(phase)
                {
                case 2:         //list of game for the player
                {
                    jsonResponse["games"] = jsonArrayOfGamesForUidPlayer(m_uid);
                }
                    break;
                case 10:        //list of all players
                {
                    QList<QString> listOfPlayers = Authentification::listOfAllUsers();
                    jsonResponse["players"] = QJsonArray::fromStringList(listOfPlayers);
                    jsonResponse["count"] = listOfPlayers.count();
                }
                    break;

                case 11:        //create a new game
                {
                    int uidOtherPlayer = jsonReceived["uidOtherPlayer"].toInt();
                    QString nameOfTheGame = jsonReceived["name"].toString();
                    InstanceManager::instance()->createNewGame(m_uid, uidOtherPlayer, nameOfTheGame);
                }
                    break;

                case 12:        //remove a game
                {
                    int uidGame = jsonReceived["uidGame"].toInt();
                    InstanceManager::instance()->removeGame(uidGame);
                }
                    break;

                default:
                    m_tcpSocket->write("error: phase does not exist");
                    qCritical() << "error: phase does not exist";
                }
            }
            //wrong token
            else
            {
                m_tcpSocket->write("wrong token");
                qCritical() << "wrong token";
            }

        }

    }
    else
    {
        m_tcpSocket->write("error during the creation of the json document");
        qCritical() << "error during the creation of the json document";
    }

    /*QProcess *process = InstanceManager::instance()->process(0);

    if(process != nullptr)
    {
        QEventLoop loop;
        connect(process, &QProcess::readyRead, &loop, &QEventLoop::quit);
        //connect(process, &QProcess::readyRead, this, &ThreadClient::onReadyRead_Process);
        process->write(message + "\n");

        qDebug() << "Debut de l'attente";
        loop.exec();
        qDebug() << "Fin de l'attente";

        QByteArray messageReceived = process->readLine();
        qDebug() << "Message received from instance:" << messageReceived;
        m_tcpSocket->write(messageReceived);
    }
    else
    {
        qCritical() << "Process is nullptr";
    }*/

}

void ThreadClient::onDisconnected_TcpSocket()
{
    m_tcpSocket->deleteLater();
    exit(0);
}

void ThreadClient::onReadyRead_Process()
{
    qDebug() << __PRETTY_FUNCTION__ << "Message recu du process";
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
QJsonObject ThreadClient::authentify(QString user, QString password)
{
    QJsonObject jsonResponse;
    Authentification auth;

    if(auth.checkUser(user, password))
    {
        //set data
        m_user = auth.user();
        m_uid = auth.uid();
        m_token = auth.token();

        //answer
        jsonResponse["result"] = "ok";
        jsonResponse["token"] = m_token;
        jsonResponse["games"] = jsonArrayOfGamesForUidPlayer(m_uid);
    }
    else
    {
        jsonResponse["result"] = "ko";
        jsonResponse["error"] = "authentification failed";
    }

    return jsonResponse;
}

QJsonArray ThreadClient::jsonArrayOfGamesForUidPlayer(int uidPlayer)
{
    QJsonArray jsonListIdGames;
    QList<int> listUidGames = InstanceManager::instance()->listUidGamesFromUidPlayer(uidPlayer);
    foreach(int uidGame, listUidGames)
    {
        QJsonObject jsonGame;

        jsonGame["uid"] = uidGame;
        jsonGame["name"] = InstanceManager::instance()->nameOfTheGameFromUidGame(uidGame);

        jsonListIdGames.append(jsonGame);
    }

    return jsonListIdGames;
}
