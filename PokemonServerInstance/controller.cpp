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
#include "src_Models/modellistenergies.h"
#include "src_Packets/fightarea.h"
#include "../Share/constantesshared.h"

Controller::Controller(const QString &nameGame, const QString &player1, const QString &player2, QObject *parent) :
    QObject(parent),
    m_communication(new StdListenerWritter()),
    m_gameManager(GameManager::createInstance()),
    m_historicNotif(HistoricalNotifications()),
    m_log(Log(nameGame))
{
    qDebug() << "Constructeur Controller";
    connect(m_communication, &StdListenerWritter::messageReceived, this, &Controller::onMessageReceived_Communication);
    connect(m_communication, &StdListenerWritter::logReceived, this, &Controller::onLogReceived);
    m_communication->startListening();

    connect(m_gameManager, &GameManager::indexCurrentPlayerChanged, this, &Controller::onIndexCurrentPlayerChanged_GameManager);
    connect(m_gameManager, &GameManager::dataPokemonChanged, this, &Controller::onDataPokemonChanged_GameManager);
    connect(m_gameManager, &GameManager::energyAdded, this, &Controller::onEnergyAdded_GameManager);
    connect(m_gameManager, &GameManager::energyRemoved, this, &Controller::onEnergyRemoved_GameManager);
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
        QJsonObject jsonResponseOwner;
        unsigned int readPoint = m_historicNotif.readPoint();
        QJsonValue valuePhase = jsonReceived["phase"];

        if(!valuePhase.isNull())
        {
            int phase = valuePhase.toInt();
            m_log.write("Message - Phase= " + QString::number(phase));

            switch(static_cast<ConstantesShared::GamePhase>(phase))
            {
            case ConstantesShared::PHASE_SelectCards:
                jsonResponseOwner = selectCardPerPlayer(jsonReceived["namePlayer"].toString(),
                                                   jsonReceived["cards"].toArray());
                break;

            case ConstantesShared::PHASE_InitReady:
                jsonResponseOwner = setInitReadyForAPlayer(jsonReceived["namePlayer"].toString());
                break;

            case ConstantesShared::PHASE_MoveACard:
                jsonResponseOwner = moveACard(jsonReceived["namePlayer"].toString(),
                                         static_cast<Player::EnumPacket>(jsonReceived["idPacketOrigin"].toInt()),
                                         static_cast<Player::EnumPacket>(jsonReceived["idPacketDestination"].toInt()),
                                         jsonReceived["idCardOrigin"].toInt(),
                                         jsonReceived["idCardDestination"].toInt());
                break;

            case ConstantesShared::PHASE_Attack_Retreat:
                jsonResponseOwner = attack_retreat(jsonReceived["namePlayer"].toString(),
                                                   jsonReceived["indexAttack"].toInt());
                break;

            case ConstantesShared::PHASE_SkipTheTurn:
                jsonResponseOwner = skipTurn(jsonReceived["namePlayer"].toString());
                break;

            default:
                const QString error = "error with the phase:" + QString::number(phase);
                jsonResponseOwner["success"] = "ko";
                jsonResponseOwner["error"] = error;
                m_log.write(QString(__PRETTY_FUNCTION__) + ", error: " + error);
            }

            jsonResponseOwner["phase"] = jsonReceived["phase"];
        }
        else
        {
            const QString error = "No phase entered";
            jsonResponseOwner["success"] = "ko";
            jsonResponseOwner["error"] = error;
            m_log.write(QString(__PRETTY_FUNCTION__) + ", error: " + error);
        }

        //On envoit la réponse avec/sans les actions en fonction de si tout s'est bien passé ou non
        if(jsonResponseOwner["success"] == "ok")
        {
            QJsonObject objOwner = m_historicNotif.buildJsonOwnerFrom(readPoint);
            jsonResponseOwner["actions"] = objOwner;

            m_communication->write(jsonReceived["namePlayer"].toString().toLatin1() + ";" +
                                    QJsonDocument(jsonResponseOwner).toJson(QJsonDocument::Compact) + ";" +
                                    QJsonDocument(m_historicNotif.buildJsonOthersFrom(readPoint)).toJson(QJsonDocument::Compact));
        }
        else
        {
            m_communication->write(jsonReceived["namePlayer"].toString().toLatin1() + ";" +
                                    QJsonDocument(jsonResponseOwner).toJson(QJsonDocument::Compact));
        }
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

void Controller::onDataPokemonChanged_GameManager(const QString& namePlayer, ConstantesShared::EnumPacket packet, int indexCard, CardPokemon* pokemon)
{
    sendNotifDataPokemonChanged(namePlayer, packet, indexCard, pokemon);
}

void Controller::onEnergyAdded_GameManager(const QString& namePlayer, ConstantesShared::EnumPacket packet, int indexCard, int idEnergy)
{
    sendNotifEnergyAdded(namePlayer, packet, indexCard, idEnergy);
}

void Controller::onEnergyRemoved_GameManager(const QString& namePlayer, ConstantesShared::EnumPacket packet, int indexCard, int indexEnergy)
{
    sendNotifEnergyRemoved(namePlayer, packet, indexCard, indexEnergy);
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
        jsonResponse["success"] = "ko";
        jsonResponse["error"] = "No enough card in packet";
        m_log.write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": No enough card in packet");
    }
    else if(listCards.count() > MAXCARDS_DECK)
    {
        jsonResponse["success"] = "ko";
        jsonResponse["error"] = "Too many cards in packet";
        m_log.write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": Too many cards in packet");
    }
    else
    {
        Player* play = m_gameManager->addNewPlayer(namePlayer, listCards);

        if(play != nullptr)
        {
            jsonResponse["success"] = "ok";
            m_log.write(QString(__PRETTY_FUNCTION__) + ", " + namePlayer + " created");
        }
        else
        {
            jsonResponse["success"] = "ko";
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
        jsonResponse["success"] = "ok";
    }
    else
    {
        jsonResponse["success"] = "ko";
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
            jsonResponse["success"] = "ok";
            sendNotifPlayerIsReady();
        }
        else
        {
            jsonResponse["success"] = "ko";
            jsonResponse["error"] = "error, no pokemon in fight area";
        }
    }
    else
    {
        jsonResponse["success"] = "ko";
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

            if(success == true)
                jsonResponse["success"] = "ok";
            else
            {
                qCritical() << __PRETTY_FUNCTION__ << "Erreur lors de l'échange";
                jsonResponse["success"] = "ko";
                jsonResponse["error"] = "error during the retreat";
            }
        }
        else
        {
            qCritical() << __PRETTY_FUNCTION__ << "erreur de indexAttack=" << indexAttack;
            jsonResponse["success"] = "ko";
            jsonResponse["error"] = "error of indexAttack = " + QString::number(indexAttack);
        }
    }
    else
    {
        jsonResponse["success"] = "ko";
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
        jsonResponse["success"] = "ok";
    }
    else
    {
        jsonResponse["success"] = "ko";
        jsonResponse["error"] = "not your turn";
    }

    return jsonResponse;
}

void Controller::sendNotifPlayerIsReady()
{
    QMap<QString, bool> mapListPlayersReady;
    foreach(Player* play, m_gameManager->listOfPlayers())
    {
        mapListPlayersReady.insert(play->name(), play->initReady());
    }

    AbstractNotification* notif = new NotificationPlayerIsReady(m_gameManager->gameStatus() == ConstantesQML::StepGameInProgress,   //bool everyoneIsReady
                                                                mapListPlayersReady);                                               //QMap listPlayersReady
    m_historicNotif.addNewNotification(notif);
}

void Controller::sendNotifEndOfTurn(const QString &oldPlayer, const QString &newPlayer)
{
    AbstractNotification* notif = new NotificationEndOfTurn(oldPlayer, newPlayer);
    m_historicNotif.addNewNotification(notif);
}

void Controller::sendNotifCardMoved(const QString &namePlayer, ConstantesShared::EnumPacket packetOrigin, int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, int indexCardDestination, int idCard)
{
    AbstractNotification* notif = new NotificationCardMoved(namePlayer,
                                                            idCard,
                                                            packetOrigin,
                                                            indexCardOrigin,
                                                            packetDestination,
                                                            indexCardDestination);
    m_historicNotif.addNewNotification(notif);
}

void Controller::sendNotifDataPokemonChanged(const QString &namePlayer, ConstantesShared::EnumPacket packet, int indexCard, CardPokemon *pokemon)
{
    QMap<unsigned int, bool> mapAttackAvailable;
    for(int i=0;i<pokemon->attacksCount();++i)
    {
        mapAttackAvailable.insert(i, pokemon->numberOfTurnAttackStillBlocks(i) > 0);
    }

    QList<unsigned int> listEnergies;
    for(unsigned short i=0;i<pokemon->modelListOfEnergies()->countEnergies();++i)
    {
        listEnergies.append(pokemon->modelListOfEnergies()->energy(i)->id());
    }

    AbstractNotification* notif = new NotificationDataPokemonChanged(namePlayer,
                                                                     packet,
                                                                     indexCard,
                                                                     pokemon->lifeLeft(),
                                                                     mapAttackAvailable,
                                                                     listEnergies);
    m_historicNotif.addNewNotification(notif);
}

void Controller::sendNotifPokemonSwitched(const QString &namePlayer, ConstantesShared::EnumPacket packet, int indexCard, int newIdCard)
{
    AbstractNotification* notif = new NotificationPokemonSwitched(namePlayer,
                                                                  packet,
                                                                  indexCard,
                                                                  newIdCard);

    m_historicNotif.addNewNotification(notif);
}

void Controller::sendNotifEnergyAdded(const QString &namePlayer, ConstantesShared::EnumPacket packet, int indexCard, int idEnergy)
{
    AbstractNotification* notif = new NotificationEnergyAdded(namePlayer,
                                                              packet,
                                                              indexCard,
                                                              idEnergy);
    m_historicNotif.addNewNotification(notif);
}

void Controller::sendNotifEnergyRemoved(const QString &namePlayer, ConstantesShared::EnumPacket packet, int indexCard, int indexEnergy)
{
    AbstractNotification* notif = new NotificationEnergyRemoved(namePlayer,
                                                                packet,
                                                                indexCard,
                                                                indexEnergy);
    m_historicNotif.addNewNotification(notif);
}

void Controller::sendNotifDisplayPacket(const QString &namePlayer, QMap<int, int> cards)
{
    AbstractNotification* notif = new NotificationDisplayPackets(namePlayer,
                                                                 cards);

    m_historicNotif.addNewNotification(notif);
}
