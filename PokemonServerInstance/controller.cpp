#include "controller.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "gamemanager.h"
#include "common/database.h"
#include "common/utils.h"
#include "src_Cards/abstractcard.h"
#include "src_Communication/stdlistenerwritter.h"
#include "src_Packets/fightarea.h"
#include "../Share/constantesshared.h"

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
void Controller::onMessageReceived_Communication(QString message)
{
    m_log.write("Message received: " + message);

    QJsonDocument jsonReceived = QJsonDocument::fromJson(message.toLatin1());

    if(!jsonReceived.isEmpty())
    {
        QJsonObject jsonResponse;
        QJsonValue valuePhase = jsonReceived["phase"];

        if(!valuePhase.isNull())
        {
            int phase = valuePhase.toInt();

            switch(static_cast<ConstantesShared::GamePhase>(phase))
            {
            case ConstantesShared::PHASE_SelectCards:
                jsonResponse = selectCardPerPlayer(jsonReceived["namePlayer"].toString(),
                                                   jsonReceived["cards"].toArray());
                break;

            case ConstantesShared::PHASE_InitReady:
                jsonResponse = setInitReadyForAPlayer(jsonReceived["namePlayer"].toString());
                break;

            case ConstantesShared::PHASE_MoveACard:
                jsonResponse = moveACard(jsonReceived["namePlayer"].toString(),
                                         static_cast<Player::EnumPacket>(jsonReceived["idPacketOrigin"].toInt()),
                                         static_cast<Player::EnumPacket>(jsonReceived["idPacketDestination"].toInt()),
                                         jsonReceived["idCardOrigin"].toInt(),
                                         jsonReceived["idCardDestination"].toInt());
                break;

            case ConstantesShared::PHASE_Attack_Retreat:
                jsonResponse = attack_retreat(jsonReceived["namePlayer"].toString(),
                                              jsonReceived["indexAttack"].toInt());
                break;

            case ConstantesShared::PHASE_SkipTheTurn:
                jsonResponse = skipTurn(jsonReceived["namePlayer"].toString());
                break;

            default:
                m_log.write(QString(__PRETTY_FUNCTION__) + ", error with the phase:" + QString::number(phase));
            }
        }
        else
        {
            jsonResponse["result"] = "ko";
            jsonResponse["error"] = "No phase entered";
        }

        m_communication->write(QJsonDocument(jsonResponse).toJson());
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
                    m_log.write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": id unknown (" + QString::number(idCard) + ")");
                }
            }
        }
        else
        {
            m_log.write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": valueCard is not an jsonObject");
        }
    }

    //Check the packet is ok
    if(listCards.count() < MAXCARDS_DECK)
    {
        jsonResponse["result"] = "ko";
        jsonResponse["error"] = "No enough card in packet";
        m_log.write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": No enough card in packet");
    }
    else if(listCards.count() > MAXCARDS_DECK)
    {
        jsonResponse["result"] = "ko";
        jsonResponse["error"] = "Too many cards in packet";
        m_log.write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": Too many cards in packet");
    }
    else
    {
        Player* play = m_gameManager->addNewPlayer(namePlayer, listCards);

        if(play != nullptr)
        {
            jsonResponse["result"] = "ok";
            m_log.write(QString(__PRETTY_FUNCTION__) + ", " + namePlayer + " created");
        }
        else
        {
            jsonResponse["result"] = "ko";
            jsonResponse["error"] = "Too many players in the game";
            m_log.write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": Too many players in the game");
        }
    }

    return jsonResponse;
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

QJsonObject Controller::setInitReadyForAPlayer(const QString &namePlayer)
{
    QJsonObject jsonResponse;
    Player* playerReady = m_gameManager->playerByName(namePlayer);

    if(playerReady != nullptr)
    {
        m_gameManager->setInitReady(playerReady);
        if(playerReady->initReady())
        {
            jsonResponse["result"] = "ok";
            sendNotifPlayerIsReady(namePlayer);
        }
        else
        {
            jsonResponse["result"] = "ko";
            jsonResponse["error"] = "error, no pokemon in fight area";
        }
    }
    else
    {
        jsonResponse["result"] = "ko";
        jsonResponse["error"] = "error, name of player not found";
    }

    return jsonResponse;
}

QJsonObject Controller::attack_retreat(const QString &namePlayer, unsigned short indexAttack)
{
    QJsonObject jsonResponse;
    Player* currentPlayer = m_gameManager->playerByName(namePlayer);

    if(currentPlayer == m_gameManager->currentPlayer())
    {
        CardPokemon* pokemonAttacking = currentPlayer->fight()->pokemonFighter();

        if(indexAttack < 4)         //Correspond à une attaque
        {
            //Le pokémon attaquant attaque
            CardPokemon::Enum_StatusOfAttack statusOfAttack = m_gameManager->attack(pokemonAttacking, indexAttack);

            switch(statusOfAttack)
            {
            case CardPokemon::Attack_OK:
                m_gameManager->endOfTurn();
                break;
            case CardPokemon::Attack_AttackBlocked:
                m_gameManager->displayMessage("Impossible d'utiliser cette attaque pendant ce tour");
                break;
            case CardPokemon::Attack_NotEnoughEnergies:
                m_gameManager->displayMessage("Vous n'avez pas assez d'énergies");
                break;
            case CardPokemon::Attack_WrongStatus:
                m_gameManager->displayMessage("Vous ne pouvez pas attaquer pendant que vous êtes " + pokemonAttacking->statusFormatString());
                break;
            case CardPokemon::Attack_IndexNOK:
                qCritical() << __PRETTY_FUNCTION__ << "Erreur d'index";
                break;
            case CardPokemon::Attack_UnknownError:
                qCritical() << __PRETTY_FUNCTION__ << "Erreur inconnue";
                break;
            }

        }
        else if(indexAttack == 4)   //Correspond à la retraite
        {
            bool success = m_gameManager->retreat(pokemonAttacking);

            if(success == false)
                qCritical() << __PRETTY_FUNCTION__ << "Erreur lors de l'échange";
        }
        else
        {
            qCritical() << __PRETTY_FUNCTION__ << "erreur de indexAttack=" << indexAttack;
        }
    }
    else
    {
        jsonResponse["result"] = "ko";
        jsonResponse["error"] = "not your turn";
    }


    return jsonResponse;
}

QJsonObject Controller::skipTurn(const QString &namePlayer)
{
    QJsonObject jsonResponse;
    Player* currentPlayer = m_gameManager->playerByName(namePlayer);

    if(currentPlayer == m_gameManager->currentPlayer())
    {
        m_gameManager->endOfTurn();
    }
    else
    {
        jsonResponse["result"] = "ko";
        jsonResponse["error"] = "not your turn";
    }

    return jsonResponse;
}

void Controller::sendNotifPlayerIsReady(const QString &namePlayer)
{
    QJsonObject jsonResponse;

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifPlayerIsReady);
    jsonResponse["namePlayer"] = namePlayer;

    m_communication->write(QJsonDocument(jsonResponse).toJson());
}
