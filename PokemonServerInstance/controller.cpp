#include "controller.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "gamemanager.h"
#include "common/database.h"
#include "common/utils.h"
#include "src_Communication/stdlistenerwritter.h"
#include "src_Log/log.h"
#include "src_Cards/abstractcard.h"

Controller::Controller(const QString &nameGame, const QString &player1, const QString &player2, QObject *parent) :
    QObject(parent),
    m_communication(new StdListenerWritter()),
    m_gameManager(GameManager::createInstance()),
    m_log(Log(nameGame))
{
    connect(m_communication, &StdListenerWritter::messageReceived, this, &Controller::onMessageReceived_Communication);

    m_log.write("Creation of the game");
    m_log.write("Players: " + player1 + " versus " + player2);
}

/************************************************************
*****			FONCTIONS SLOTS PRIVEES					*****
************************************************************/
void Controller::onMessageReceived_Communication(QByteArray message)
{
    m_log.write("Message received: " + message);

    QJsonDocument jsonReceived = QJsonDocument::fromJson(message);

    if(!jsonReceived.isEmpty())
    {
        QJsonObject jsonResponse;
        int phase = jsonReceived["phase"].toInt();

        switch(phase)
        {
        case 31:        //select card
            jsonResponse = selectCardPerPlayer(jsonReceived["name"].toString(),
                                               jsonReceived["cards"].toArray());
            break;

        case 32:
            jsonResponse = moveACard(jsonReceived["name"].toString(),
                                     static_cast<Player::EnumPacket>(jsonReceived["idPacketOrigin"].toInt()),
                                     static_cast<Player::EnumPacket>(jsonReceived["idPacketDestination"].toInt()),
                                     jsonReceived["idCardOrigin"].toInt(),
                                     jsonReceived["idCardDestination"].toInt());
            break;

        default:
            m_log(__PRETTY_FUNCTION__ + ", error with the phase:" + QString::number(phase));
        }

    }
}

/************************************************************
*****               FONCTIONS PRIVEES					*****
************************************************************/
QJsonObject Controller::selectCardPerPlayer(const QString &namePlayer, QJsonArray tabCards)
{
    QJsonObject jsonResponse;
    QList<AbstractCard*> listCards;
    Database db;

    //Fill the packet
    for(int indexTabCards=0;indexTabCards<tabCards.count();++indexTabCards)
    {
        QJsonValue valueCard = tabCards.at(indexTabCards);

        if(valueCard.isObject())
        {
            QJsonObject objectCard = valueCard.toObject();
            int idCard = objectCard["id"].toInt();
            int quantity = objectCard["quantity"].toInt();

            for(int indexQuantity=0;indexQuantity<quantity;++indexQuantity)
            {
                AbstractCard* abCard = db.cardById(idCard);

                if(abCard != nullptr)
                {
                    listCards.append(abCard);
                }
                else
                {
                    m_log.write(__PRETTY_FUNCTION__ + ", error for " + namePlayer + ": id unknown (" + QString::number(idCard) + ")");
                }
            }
        }
        else
        {
            m_log.write(__PRETTY_FUNCTION__ + ", error for " + namePlayer + ": valueCard is not an jsonObject");
        }
    }

    //Check the packet is ok
    if(listCards.count() < MAXCARDS_DECK)
    {
        jsonResponse["result"] = "ko";
        jsonResponse["error"] = "No enough card in packet";
        m_log.write(__PRETTY_FUNCTION__ + ", error for " + namePlayer + ": No enough card in packet");
    }
    else if(listCards.count() > MAXCARDS_DECK)
    {
        jsonResponse["result"] = "ko";
        jsonResponse["error"] = "Too many cards in packet";
        m_log.write(__PRETTY_FUNCTION__ + ", error for " + namePlayer + ": Too many cards in packet");
    }
    else
    {
        Player* play = m_gameManager->addNewPlayer(namePlayer, listCards);

        if(play != nullptr)
        {
            jsonResponse["result"] = "ok";
            m_log.write(__PRETTY_FUNCTION__ + ", " + namePlayer + " created");
        }
        else
        {
            jsonResponse["result"] = "ko";
            jsonResponse["error"] = "Too many players in the game";
            m_log.write(__PRETTY_FUNCTION__ + ", error for " + namePlayer + ": Too many players in the game");
        }
    }
}

QJsonObject Controller::moveACard(const QString &namePlayer, Player::EnumPacket packetOrigin, Player::EnumPacket packetDestination, int indexCardOrigin, int indexCardDestination)
{
    QJsonObject jsonResponse;

    if(m_gameManager->moveACard(namePlayer, packetOrigin, packetDestination, indexCardOrigin, indexCardDestination))
    {
        jsonResponse["result"] = "ok";
    }
    else
    {
        jsonResponse["result"] = "ko";
        jsonResponse["error"] = "error during the move of the card";
    }

    return jsonResponse;
}
