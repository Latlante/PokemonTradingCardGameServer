#include "controller.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "gamemanager.h"
#include "common/constantesqml.h"
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
    qDebug() << "Constructeur Controller";
    connect(m_communication, &StdListenerWritter::messageReceived, this, &Controller::onMessageReceived_Communication);
    connect(m_communication, &StdListenerWritter::logReceived, this, &Controller::onLogReceived);
    m_communication->startListening();

    connect(m_gameManager, &GameManager::indexCurrentPlayerChanged, this, &Controller::onIndexCurrentPlayerChanged_GameManager);
    connect(m_gameManager, &GameManager::logReceived, this, &Controller::onLogReceived);
    m_gameManager->setNumberMaxOfPlayers(2);

    m_log.write("Creation of the game");
    m_log.write("Players: " + player1 + " versus " + player2);
}

Controller::~Controller()
{
    qDebug() << "Destructeur Controller";
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
            m_log.write("Message - Phase= " + QString::number(phase));

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

            jsonResponse["namePlayer"] = jsonReceived["namePlayer"];
            jsonResponse["phase"] = jsonReceived["phase"];
        }
        else
        {
            const QString error = "No phase entered";
            jsonResponse["result"] = "ko";
            jsonResponse["error"] = error;
            m_log.write(QString(__PRETTY_FUNCTION__) + ", error: " + error);
        }

        m_communication->write(jsonReceived["namePlayer"] + ";" + QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact));
    }
    else
    {
        m_log.write(QString(__PRETTY_FUNCTION__) + ", error: jsonReceived is empty");
    }

}

void Controller::onLogReceived(QString message)
{
    m_log.write(message);
}

void Controller::onIndexCurrentPlayerChanged_GameManager(const QString &oldPlayer, const QString &newPlayer)
{
    sendNotifEndOfTurn(oldPlayer, newPlayer);
}

/************************************************************
*****               FONCTIONS PRIVEES					*****
************************************************************/
QJsonObject Controller::selectCardPerPlayer(const QString &namePlayer, QJsonArray tabCards)
{
    QJsonObject jsonResponse;
    QList<AbstractCard*> listCards;
    Database db;

    m_log.write(QString(__PRETTY_FUNCTION__) + ", namePlayer=" + namePlayer + ", tabCards.count=" + QString::number(tabCards.count()));

    //Fill the packet
    for(int indexTabCards=0;indexTabCards<tabCards.count();++indexTabCards)
    {
        QJsonValue valueCard = tabCards.at(indexTabCards);

        if(valueCard.isObject())
        {
            QJsonObject objectCard = valueCard.toObject();
            int idCard = objectCard["id"].toInt();
            int quantity = objectCard["quantity"].toInt();

            m_log.write(QString(__PRETTY_FUNCTION__) + ", idCard=" + QString::number(idCard) + ", quantity=" + QString::number(quantity));

            for(int indexQuantity=0;indexQuantity<quantity;++indexQuantity)
            {
                AbstractCard* abCard = db.cardById(idCard);

                if(abCard != nullptr)
                {
                    m_log.write(QString(__PRETTY_FUNCTION__) + ", abCard.name=" + abCard->name());
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
            jsonResponse["error"] = "Impossible to add a new player";
            m_log.write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": Impossible to add a new player");
        }
    }

    return jsonResponse;
}

QJsonObject Controller::moveACard(const QString &namePlayer, Player::EnumPacket packetOrigin, Player::EnumPacket packetDestination, int indexCardOrigin, int indexCardDestination)
{
    QJsonObject jsonResponse;

    m_log.write(QString(__PRETTY_FUNCTION__));

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

    m_log.write(QString(__PRETTY_FUNCTION__));

    if(playerReady != nullptr)
    {
        m_gameManager->setInitReady(playerReady);
        if(playerReady->initReady())
        {
            jsonResponse["result"] = "ok";
            sendNotifPlayerIsReady();
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

void Controller::sendNotifPlayerIsReady()
{
    QByteArray messageToRespond;
    QJsonObject jsonResponse;

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifPlayerIsReady);
    jsonResponse["everyoneIsReady"] = m_gameManager->gameStatus() == ConstantesQML::StepGameInProgress;

    QJsonArray arrayPlayers;
    foreach(Player* play, m_gameManager->listOfPlayers())
    {
        QJsonObject objPlayer;
        objPlayer["namePlayer"] = play->name();
        objPlayer["ready"] = play->initReady();

        arrayPlayers.append(objPlayer);
    }
    jsonResponse["players"] = arrayPlayers;

    //no player concerned ; no message for the owner ; message for everyone
    messageToRespond = ";;" + QJsonDocument(jsonResponse).toJson();
    m_communication->write(messageToRespond);
}

void Controller::sendNotifEndOfTurn(const QString &oldPlayer, const QString &newPlayer)
{
    QByteArray messageToRespond;
    QJsonObject jsonResponse;

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifEndOfTurn);
    jsonResponse["endOfTurn"] = oldPlayer;
    jsonResponse["newTurn"] = newPlayer;

    //no player concerned ; no message for the owner ; message for everyone
    messageToRespond = ";;" + QJsonDocument(jsonResponse).toJson();
    m_communication->write(messageToRespond);
}

void Controller::sendNotifCardMoved(const QString &namePlayer, ConstantesShared::EnumPacket packetOrigin, int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, int indexCardDestination, int idCard)
{
    QByteArray messageToRespond;
    QJsonObject jsonResponse;

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifCardMoved);
    jsonResponse["namePlayer"] = namePlayer;
    jsonResponse["indexAction"] = 0;
    jsonResponse["idPacketOrigin"] = static_cast<int>(packetOrigin);
    jsonResponse["indexCardOrigin"] = indexCardOrigin;
    jsonResponse["idPacketDestination"] = static_cast<int>(packetDestination);
    jsonResponse["indexCardDestination"] = indexCardDestination;

    //For others
    QByteArray messageOthers = QJsonDocument(jsonResponse).toJson();

    //For owner
    jsonResponse["idCard"] = idCard;

    //player who move the card ; info of move with the id of the card ; only info of move
    messageToRespond = namePlayer + ";" + QJsonDocument(jsonResponse).toJson() + ";" + messageOthers;
    m_communication->write(QJsonDocument(jsonResponse).toJson());
}

void Controller::sendNotifDataPokemonChanged(const QString &namePlayer, ConstantesShared::EnumPacket packet, int indexCard, CardPokemon *pokemon)
{
    QByteArray messageToRespond;
    QJsonObject jsonResponse;

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifDataPokemonChanged);
    jsonResponse["namePlayer"] = namePlayer;
    jsonResponse["indexAction"] = 0;
    jsonResponse["idPacket"] = static_cast<int>(packet);
    jsonResponse["indexCard"] = indexCard;
    jsonResponse["lifeLeft"] = pokemon->lifeLeft();

    QJsonArray arrayAttacks;
    for(int i=0;i<pokemon->attacksCount();++i)
    {
        QJsonObject objAttack;
        objAttack["index"] = i;
        objAttack["available"] = pokemon->numberOfTurnAttackStillBlocks(i) > 0;

        arrayAttacks.append(objAttack);
    }
    jsonResponse["attacks"] = arrayAttacks;

    //no player concerned ; no message for the owner ; message for everyone
    messageToRespond = ";;" + QJsonDocument(jsonResponse).toJson();
    m_communication->write(messageToRespond);
}
