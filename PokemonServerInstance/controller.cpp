#include "controller.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "gamemanager.h"
#include "common/constantesqml.h"
#include "common/database.h"
#include "common/utils.h"
#include "src_Communication/sockettoserver.h"
#include "src_Communication/localsockettoserver.h"
#include "src_Displays/displaydata_allelements.h"
#include "src_Displays/displaydata_attackspokemon.h"
#include "src_Displays/displaydata_energiesforapokemon.h"
#include "src_Displays/displaydata_packet.h"
#include "src_Displays/displaydata_hiddenpacket.h"
#include "src_Models/modellistenergies.h"
#include "src_Packets/bencharea.h"
#include "src_Packets/fightarea.h"
#include "src_Packets/packetdeck.h"
#include "src_Packets/packethand.h"
#include "src_Packets/packetrewards.h"
#include "src_Packets/packettrash.h"
#include "../Share/constantesshared.h"
#include <QCoreApplication>

Controller::Controller(const QString &uidGame, const QString &nameGame, const QString &player1, const QString &player2, QObject *parent) :
    QObject(parent),
    m_communication(new LocalSocketToServer()),
    m_gameManager(GameManager::createInstance()),
    m_historicNotif(HistoricalNotifications()),
    m_displayData(nullptr)
{
    Log::createInstance(nameGame);
    Log::instance()->write("Constructeur Controller");

    if(m_communication->tryToConnect())
    {
        connect(m_communication, &LocalSocketToServer::messageReceived, this, &Controller::onMessageReceived_Communication);

        //Authentification for the server
        /*QString requestBoot = "boot;";
        requestBoot += "{";
        requestBoot += "\"uidGame\":" + uidGame.toLatin1() + ",";
        requestBoot += "\"nameGame\":\"" + nameGame + "\",";
        requestBoot += "\"namePlayer1\":\"" + player1 + "\",";
        requestBoot += "\"namePlayer2\":\"" + player2 + "\"";
        requestBoot += "}";
        m_communication->write(requestBoot.toLatin1());*/

        NotificationAuthentificationServer notifAuthentification("boot", uidGame, nameGame, player1, player2);
        m_communication->write(notifAuthentification.messageJsonComplete());

        //Notification for the player 2
        sendNotifNewGameCreated(uidGame.toInt(), nameGame, player1, player2);
    }
    else
        Log::instance()->write("Error: Impossible to connect to the server");


    connect(m_gameManager, &GameManager::indexCurrentPlayerChanged, this, &Controller::onIndexCurrentPlayerChanged_GameManager);
    connect(m_gameManager, &GameManager::initReadyChanged, this, &Controller::onInitReadyChanged_GameManager);
    connect(m_gameManager, &GameManager::cardMoved, this, &Controller::onCardMoved_GameManager);
    connect(m_gameManager, &GameManager::dataPokemonChanged, this, &Controller::onDataPokemonChanged_GameManager);
    connect(m_gameManager, &GameManager::pokemonSwitched, this, &Controller::onPokemonSwitched_GameManager);
    connect(m_gameManager, &GameManager::energyAdded, this, &Controller::onEnergyAdded_GameManager);
    connect(m_gameManager, &GameManager::energyRemoved, this, &Controller::onEnergyRemoved_GameManager);
    connect(m_gameManager, &GameManager::headOrTailDone, this, &Controller::onHeadOrTailDone_GameManager);

    connect(m_gameManager, &GameManager::displayPacketAsked, this, &Controller::onDisplayPacketAsked);
    connect(m_gameManager, &GameManager::displayAllElementsAsked, this, &Controller::onDisplayAllElementsAsked);
    connect(m_gameManager, &GameManager::displaySelectHiddenCardAsked, this, &Controller::onDisplayHiddenPacketAsked);
    connect(m_gameManager, &GameManager::displayEnergiesForAPokemonAsked, this, &Controller::onDisplayEnergiesForAPokemonAsked);
    connect(m_gameManager, &GameManager::displayAttacksAsked, this, &Controller::onDisplayAttacksAsked);

    m_gameManager->setNumberMaxOfPlayers(2);

    Log::instance()->write("Creation of the game");
    Log::instance()->write("Players: " + player1 + " versus " + player2);

    m_gameManager->addNewPlayer(player1);
    m_gameManager->addNewPlayer(player2);
}

Controller::~Controller()
{
    Log::instance()->write("Destructeur Controller");
    qDebug() << "Destructeur Controller";

    if(m_displayData != nullptr)
    {
        delete m_displayData;
        m_displayData = nullptr;
    }

    delete m_communication;
    GameManager::deleteInstance();
    Log::deleteInstance();
}

/************************************************************
*****			FONCTIONS SLOTS PRIVEES					*****
************************************************************/
void Controller::onMessageReceived_Communication(QByteArray message)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + "Message received: " + message);

    if(message == "exit")
    {
        qApp->quit();
        return;
    }

    QJsonDocument jsonReceived = QJsonDocument::fromJson(message);

    if(!jsonReceived.isEmpty())
    {
        QString nameCurrentPlayer = jsonReceived["namePlayer"].toString();
        QJsonObject jsonResponseOwner;
        unsigned int readPoint = m_historicNotif.readPoint();
        QJsonValue valuePhase = jsonReceived["phase"];

        if(!valuePhase.isNull())
        {
            int phase = valuePhase.toInt();
            Log::instance()->write("Message - Phase= " + QString::number(phase));

            switch(static_cast<ConstantesShared::GamePhase>(phase))
            {
            case ConstantesShared::PHASE_GetAllInfoOnGame:
                jsonResponseOwner = getAllInfoOnTheGame(nameCurrentPlayer);
                break;
            case ConstantesShared::PHASE_SelectCards:
                jsonResponseOwner = selectCardPerPlayer(nameCurrentPlayer,
                                                   jsonReceived["cards"].toArray());
                break;

            case ConstantesShared::PHASE_InitReady:
                jsonResponseOwner = setInitReadyForAPlayer(nameCurrentPlayer);
                break;

            case ConstantesShared::PHASE_MoveACard:
                jsonResponseOwner = moveACard(nameCurrentPlayer,
                                         static_cast<Player::EnumPacket>(jsonReceived["idPacketOrigin"].toInt()),
                                         static_cast<Player::EnumPacket>(jsonReceived["idPacketDestination"].toInt()),
                                         jsonReceived["idCardOrigin"].toInt(),
                                         jsonReceived["idCardDestination"].toInt());
                break;

            case ConstantesShared::PHASE_Attack_Retreat:
                jsonResponseOwner = attack_retreat(nameCurrentPlayer,
                                                   jsonReceived["indexAttack"].toInt());
                break;

            case ConstantesShared::PHASE_SkipTheTurn:
            {
                jsonResponseOwner = skipTurn(nameCurrentPlayer);
                //nameCurrentPlayer = nameOfEnemy(nameCurrentPlayer);
            }
                break;

            case ConstantesShared::PHASE_DisplayPacketResponse:
            case ConstantesShared::PHASE_DisplayAllElementsResponse:
            case ConstantesShared::PHASE_DisplayHiddenPacketResponse:
            case ConstantesShared::PHASE_DisplayAttakcsPokemonResponse:
                jsonResponseOwner = displayPacketResponse(jsonReceived);
                break;

            default:
                const QString error = "error with the phase:" + QString::number(phase);
                jsonResponseOwner["success"] = "ko";
                jsonResponseOwner["error"] = error;
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error: " + error);
            }

            jsonResponseOwner["phase"] = jsonReceived["phase"];
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", return from gameManager:" + QJsonDocument(jsonResponseOwner).toJson(QJsonDocument::Compact));
        }
        else
        {
            const QString error = "No phase entered";
            jsonResponseOwner["success"] = "ko";
            jsonResponseOwner["error"] = error;
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error: " + error);
        }

        //On envoit la réponse avec/sans les actions en fonction de si tout s'est bien passé ou non
        if(jsonResponseOwner["success"] == "ok")
        {
            //owner
            QJsonObject objOwner = m_historicNotif.buildJsonNotificationFrom(readPoint, nameCurrentPlayer);
            jsonResponseOwner["actions"] = objOwner;

            //opponent
            QString nameOpponent = nameOfEnemy(nameCurrentPlayer);
            QJsonObject objOpponent;
            objOpponent["actions"] = m_historicNotif.buildJsonNotificationFrom(readPoint, nameOpponent);

            //others
            QJsonObject objOther;
            objOther["actions"] = m_historicNotif.buildJsonNotificationFrom(readPoint);

            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", after notification:" + QJsonDocument(jsonResponseOwner).toJson(QJsonDocument::Compact));

            m_communication->write(nameCurrentPlayer.toLatin1() + ";" +
                                   QJsonDocument(jsonResponseOwner).toJson(QJsonDocument::Compact) + ";" +
                                   nameOpponent.toLatin1() + ";" +
                                   QJsonDocument(objOpponent).toJson(QJsonDocument::Compact) + ";" +
                                   ";" +    //no name for all other players/viewers
                                   QJsonDocument(objOther).toJson(QJsonDocument::Compact));
        }
        else
        {
            m_communication->write(nameCurrentPlayer.toLatin1() + ";" +
                                    QJsonDocument(jsonResponseOwner).toJson(QJsonDocument::Compact));
        }
    }
    else
    {
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error: jsonReceived is empty");
    }

}

void Controller::onLogReceived(QString message)
{
    Log::instance()->write(message);
}

void Controller::onInitReadyChanged_GameManager()
{
    sendNotifPlayerIsReady();
}

void Controller::onCardMoved_GameManager(const QString &namePlayer, ConstantesShared::EnumPacket packetOrigin, int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, int idCard, bool showCardToEveryone)
{
    sendNotifCardMoved(namePlayer, packetOrigin, indexCardOrigin, packetDestination, idCard, showCardToEveryone);
}

void Controller::onIndexCurrentPlayerChanged_GameManager(const QString &oldPlayer, const QString &newPlayer)
{
    sendNotifEndOfTurn(oldPlayer, newPlayer);
}

void Controller::onDataPokemonChanged_GameManager(const QString& namePlayer, ConstantesShared::EnumPacket packet, int indexCard, CardPokemon* pokemon)
{
    sendNotifDataPokemonChanged(namePlayer, packet, indexCard, pokemon);
}

void Controller::onPokemonSwitched_GameManager(const QString& namePlayer, ConstantesShared::EnumPacket packet, int indexCard, int newIdCard, bool keepEnergy)
{
    sendNotifPokemonSwitched(namePlayer, packet, indexCard, newIdCard, keepEnergy);
}

void Controller::onEnergyAdded_GameManager(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, int idEnergy)
{
    sendNotifEnergyAdded(namePlayer, packetOrigin, indexCardOrigin, packetDestination, indexCardDestination, idEnergy);
}

void Controller::onEnergyRemoved_GameManager(const QString& namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, int indexEnergy)
{
    sendNotifEnergyRemoved(namePlayer, packetOrigin, indexCardOrigin, packetDestination, indexCardDestination, indexEnergy);
}

void Controller::onHeadOrTailDone_GameManager(const QString& namePlayer, QList<unsigned short> coins)
{
    sendNotifHeadOrTailDone(namePlayer, coins);
}

void Controller::onDisplayPacketAsked(const QString &namePlayer, AbstractPacket *packet, unsigned short quantity, AbstractCard::Enum_typeOfCard typeOfCard)
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + "player:" + namePlayer + ", packet:" + packet->name() + ", quantity:" + QString::number(quantity) + ", type:" + QString::number(typeOfCard));
#endif

    m_displayData = new DisplayData_Packet(namePlayer, packet, quantity, typeOfCard);
    QJsonDocument docToSend = m_displayData->messageInfoToClient();

    Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + docToSend.toJson(QJsonDocument::Compact));
    m_communication->write(namePlayer.toLatin1() + ";" + docToSend.toJson(QJsonDocument::Compact));
}

void Controller::onDisplayAllElementsAsked(const QString &namePlayer, unsigned short quantity)
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + namePlayer + " " + QString::number(quantity));
#endif

    m_displayData = new DisplayData_AllElements(namePlayer, quantity);
    QJsonDocument docToSend = m_displayData->messageInfoToClient();

    Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + docToSend.toJson(QJsonDocument::Compact));
    if(m_communication->write(namePlayer.toLatin1() + ";" + docToSend.toJson(QJsonDocument::Compact)) == false)
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + " error to send message");
}

void Controller::onDisplayHiddenPacketAsked(const QString &namePlayer, AbstractPacket *packet, unsigned short quantity)
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + namePlayer + " " + QString::number(quantity));
#endif

    m_displayData = new DisplayData_HiddenPacket(namePlayer, packet, quantity);
    QJsonDocument docToSend = m_displayData->messageInfoToClient();

    Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + docToSend.toJson(QJsonDocument::Compact));
    if(m_communication->write(namePlayer.toLatin1() + ";" + docToSend.toJson(QJsonDocument::Compact)) == false)
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + " error to send message");
}

void Controller::onDisplayEnergiesForAPokemonAsked(const QString &namePlayer, CardPokemon* pokemon, unsigned short quantity, AbstractCard::Enum_element element)
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + namePlayer + " " + QString::number(quantity) + " " + QString::number(element));
#endif

    m_displayData = new DisplayData_EnergiesForAPokemon(namePlayer, pokemon, quantity, element);
    QJsonDocument docToSend = m_displayData->messageInfoToClient();

    Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + docToSend.toJson(QJsonDocument::Compact));
    if(m_communication->write(namePlayer.toLatin1() + ";" + docToSend.toJson(QJsonDocument::Compact)) == false)
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + " error to send message");
}

void Controller::onDisplayAttacksAsked(const QString& namePlayer, CardPokemon* pokemon, bool retreatEnable)
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + namePlayer + " " + QString::number(retreatEnable));
#endif

    if(pokemon != nullptr)
    {
        m_displayData = new DisplayData_AttacksPokemon(namePlayer, pokemon->id(), retreatEnable);
        QJsonDocument docToSend = m_displayData->messageInfoToClient();

        Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + docToSend.toJson(QJsonDocument::Compact));
        if(m_communication->write(namePlayer.toLatin1() + ";" + docToSend.toJson(QJsonDocument::Compact)) == false)
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + " error to send message");
    }
    else
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + namePlayer + ", pokemon is nullptr");
}

/************************************************************
*****               FONCTIONS PRIVEES					*****
************************************************************/
QJsonObject Controller::getAllInfoOnTheGame(const QString &namePlayer)
{
    QJsonObject jsonResponse;
    Player* playerYou = m_gameManager->playerByName(namePlayer);

    if(m_gameManager->gameStatus() == ConstantesQML::StepPreparation)
    {
        if(playerYou->deck()->countCard() <= 0)
            jsonResponse["gameStatus"] = static_cast<int>(ConstantesQML::StepSelectionCards);
        else
            jsonResponse["gameStatus"] = static_cast<int>(ConstantesQML::StepPreparation);

        //if the opponent has already fill his deck
        Player* playerOpponent = m_gameManager->enemyOf(playerYou);
        if((playerOpponent != nullptr) && (playerOpponent->deck()->countCard() > 0))
        {
            QJsonObject objEnemy;

                //Bench
            QJsonArray arrayEnemyBench;
            for(int iBench=0;iBench<playerOpponent->bench()->countCard();++iBench)
            {
                CardPokemon* pokemon = playerOpponent->bench()->cardPok(iBench);
                QJsonObject objEnemyBenchPokemon;

                objEnemyBenchPokemon["id"] = pokemon->id();
                objEnemyBenchPokemon["damage"] = pokemon->damageMarker() * DAMAGE_MARQUER_VALUE;

                    //Energies
                QJsonArray arrayEnemyBenchPokemonEnergies;
                ModelListEnergies* modelEnergies = pokemon->modelListOfEnergies();
                for(unsigned short iEnergy=0;iEnergy<modelEnergies->countEnergies();++iEnergy)
                {
                    CardEnergy* cardEn = modelEnergies->energy(iEnergy);

                    if(cardEn != nullptr)
                        arrayEnemyBenchPokemonEnergies.append(static_cast<int>(cardEn->element()));
                    else
                        Log::instance()->write(QString(__PRETTY_FUNCTION__) + "Enemy, an energy (" + QString::number(iEnergy) + ") card is nullptr");
                }

                objEnemyBenchPokemon["energies"] = arrayEnemyBenchPokemonEnergies;
                arrayEnemyBench.append(objEnemyBenchPokemon);
            }
            objEnemy["bench"] = arrayEnemyBench;

                //Deck
            objEnemy["deckCount"] = playerOpponent->deck()->countCard();

                //Fight
            QJsonObject objEnemyFight;
            CardPokemon* pokemonFight = playerOpponent->fight()->pokemonFighter();

            if(pokemonFight != nullptr)
            {
                objEnemyFight["id"] = pokemonFight->id();
                objEnemyFight["damage"] = pokemonFight->damageMarker() * DAMAGE_MARQUER_VALUE;
                objEnemyFight["status"] = static_cast<int>(pokemonFight->status());

                    //Energies
                QJsonArray arrayEnemyFightPokemonEnergies;
                ModelListEnergies* modelEnergies = pokemonFight->modelListOfEnergies();
                for(unsigned short iEnergy=0;iEnergy<modelEnergies->countEnergies();++iEnergy)
                {
                    CardEnergy* cardEn = modelEnergies->energy(iEnergy);

                    if(cardEn != nullptr)
                        arrayEnemyFightPokemonEnergies.append(static_cast<int>(cardEn->element()));
                    else
                        Log::instance()->write(QString(__PRETTY_FUNCTION__) + "Enemy, an energy (" + QString::number(iEnergy) + ") card is nullptr");
                }

                objEnemyFight["energies"] = arrayEnemyFightPokemonEnergies;
                objEnemy["fight"] = objEnemyFight;
            }
            else
            {
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + "Enemy, pokemon fighter is nullptr");
            }

                //Hand
            objEnemy["handCount"] = playerOpponent->hand()->countCard();

                //Rewards
            objEnemy["rewardsCount"] = playerOpponent->rewards()->countCard();

                //Trash
            objEnemy["trashCount"] = playerOpponent->trash()->countCard();

            jsonResponse["enemy"] = objEnemy;
        }

        jsonResponse["success"] = "ok";
    }
    else if(m_gameManager->gameStatus() == ConstantesQML::StepGameInProgress)
    {
        jsonResponse["gameStatus"] = static_cast<int>(ConstantesQML::StepGameInProgress);
        jsonResponse["success"] = "ok";

        //You
        if(playerYou != nullptr)
        {
            QJsonObject objYou;

                //Bench
            QJsonArray arrayYouBench;
            for(int iBench=0;iBench<playerYou->bench()->countCard();++iBench)
            {
                CardPokemon* pokemon = playerYou->bench()->cardPok(iBench);
                QJsonObject objYouBenchPokemon;

                objYouBenchPokemon["id"] = pokemon->id();
                objYouBenchPokemon["damage"] = pokemon->damageMarker() * DAMAGE_MARQUER_VALUE;

                    //Energies
                QJsonArray arrayYouBenchPokemonEnergies;
                ModelListEnergies* modelEnergies = pokemon->modelListOfEnergies();
                for(unsigned short iEnergy=0;iEnergy<modelEnergies->countEnergies();++iEnergy)
                {
                    CardEnergy* cardEn = modelEnergies->energy(iEnergy);

                    if(cardEn != nullptr)
                        arrayYouBenchPokemonEnergies.append(static_cast<int>(cardEn->element()));
                    else
                        Log::instance()->write(QString(__PRETTY_FUNCTION__) + "you, an energy (" + QString::number(iEnergy).toLatin1() + ") card is nullptr");
                }

                objYouBenchPokemon["energies"] = arrayYouBenchPokemonEnergies;
            }
            objYou["bench"] = arrayYouBench;

                //Deck
            objYou["deckCount"] = playerYou->deck()->countCard();

                //Fight
            QJsonObject objYouFight;
            CardPokemon* pokemonFight = playerYou->fight()->pokemonFighter();

            if(pokemonFight != nullptr)
            {
                objYouFight["id"] = pokemonFight->id();
                objYouFight["damage"] = pokemonFight->damageMarker() * DAMAGE_MARQUER_VALUE;
                objYouFight["status"] = static_cast<int>(pokemonFight->status());

                    //Energies
                QJsonArray arrayYouFightPokemonEnergies;
                ModelListEnergies* modelEnergies = pokemonFight->modelListOfEnergies();
                for(unsigned short iEnergy=0;iEnergy<modelEnergies->countEnergies();++iEnergy)
                {
                    CardEnergy* cardEn = modelEnergies->energy(iEnergy);

                    if(cardEn != nullptr)
                        arrayYouFightPokemonEnergies.append(static_cast<int>(cardEn->element()));
                    else
                        Log::instance()->write(QString(__PRETTY_FUNCTION__) + "you, an energy (" + QString::number(iEnergy) + ") card is nullptr");
                }

                objYouFight["energies"] = arrayYouFightPokemonEnergies;

            }
            else
            {
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + "you, pokemon fighter is nullptr");
            }
            objYou["fight"] = objYouFight;

                //Hand
            QJsonArray arrayYouHand;
            for(int iHand=0;iHand<playerYou->hand()->countCard();++iHand)
            {
                AbstractCard* cardHand = playerYou->hand()->card(iHand);
                if(cardHand != nullptr)
                    arrayYouHand.append(cardHand->id());
                else
                    Log::instance()->write(QString(__PRETTY_FUNCTION__) + "you, an card hand (" + QString::number(iHand) + ") is nullptr");
            }
            objYou["hand"] = arrayYouHand;

                //Rewards
            objYou["rewardsCount"] = playerYou->rewards()->countCard();

                //Trash
            objYou["trashCount"] = playerYou->trash()->countCard();

            jsonResponse["you"] = objYou;
        }

        //Enemy
        Player* playerEnemy = m_gameManager->enemyOf(playerYou);
        if(playerEnemy != nullptr)
        {
            QJsonObject objEnemy;

                //Bench
            QJsonArray arrayEnemyBench;
            for(int iBench=0;iBench<playerEnemy->bench()->countCard();++iBench)
            {
                CardPokemon* pokemon = playerEnemy->bench()->cardPok(iBench);
                QJsonObject objEnemyBenchPokemon;

                objEnemyBenchPokemon["id"] = pokemon->id();
                objEnemyBenchPokemon["damage"] = pokemon->damageMarker() * DAMAGE_MARQUER_VALUE;

                    //Energies
                QJsonArray arrayEnemyBenchPokemonEnergies;
                ModelListEnergies* modelEnergies = pokemon->modelListOfEnergies();
                for(unsigned short iEnergy=0;iEnergy<modelEnergies->countEnergies();++iEnergy)
                {
                    CardEnergy* cardEn = modelEnergies->energy(iEnergy);

                    if(cardEn != nullptr)
                        arrayEnemyBenchPokemonEnergies.append(static_cast<int>(cardEn->element()));
                    else
                        Log::instance()->write(QString(__PRETTY_FUNCTION__) + "Enemy, an energy (" + QString::number(iEnergy) + ") card is nullptr");
                }

                objEnemyBenchPokemon["energies"] = arrayEnemyBenchPokemonEnergies;
            }
            objEnemy["bench"] = arrayEnemyBench;

                //Deck
            objEnemy["deckCount"] = playerEnemy->deck()->countCard();

                //Fight
            QJsonObject objEnemyFight;
            CardPokemon* pokemonFight = playerEnemy->fight()->pokemonFighter();

            if(pokemonFight != nullptr)
            {
                objEnemyFight["id"] = pokemonFight->id();
                objEnemyFight["damage"] = pokemonFight->damageMarker() * DAMAGE_MARQUER_VALUE;
                objEnemyFight["status"] = static_cast<int>(pokemonFight->status());

                    //Energies
                QJsonArray arrayEnemyFightPokemonEnergies;
                ModelListEnergies* modelEnergies = pokemonFight->modelListOfEnergies();
                for(unsigned short iEnergy=0;iEnergy<modelEnergies->countEnergies();++iEnergy)
                {
                    CardEnergy* cardEn = modelEnergies->energy(iEnergy);

                    if(cardEn != nullptr)
                        arrayEnemyFightPokemonEnergies.append(static_cast<int>(cardEn->element()));
                    else
                        Log::instance()->write(QString(__PRETTY_FUNCTION__) + "Enemy, an energy (" + QString::number(iEnergy) + ") card is nullptr");
                }

                objEnemyFight["energies"] = arrayEnemyFightPokemonEnergies;

            }
            else
            {
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + "Enemy, pokemon fighter is nullptr");
            }
            objEnemy["fight"] = objEnemyFight;

                //Hand
            objEnemy["handCount"] = playerEnemy->hand()->countCard();

                //Rewards
            objEnemy["rewardsCount"] = playerEnemy->rewards()->countCard();

                //Trash
            objEnemy["trashCount"] = playerEnemy->trash()->countCard();

            jsonResponse["enemy"] = objEnemy;
        }

        jsonResponse["yourTurn"] = m_gameManager->currentPlayer() == playerYou ? true : false;
    }
    else
    {
        jsonResponse["gameStatus"] = static_cast<int>(ConstantesQML::StepFinished);
        jsonResponse["success"] = "ok";

    }

    return jsonResponse;
}

QJsonObject Controller::selectCardPerPlayer(const QString &namePlayer, QJsonArray tabCards)
{
    QJsonObject jsonResponse;
    QList<AbstractCard*> listCards;
    Database db;

    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", namePlayer=" + namePlayer + ", tabCards.count=" + QString::number(tabCards.count()));

    //Fill the packet
    for(int indexTabCards=0;indexTabCards<tabCards.count();++indexTabCards)
    {
        QJsonValue valueCard = tabCards.at(indexTabCards);

        if(valueCard.isObject())
        {
            QJsonObject objectCard = valueCard.toObject();
            int idCard = objectCard["id"].toInt();
            int quantity = objectCard["quantity"].toInt();

            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", idCard=" + QString::number(idCard) + ", quantity=" + QString::number(quantity));

            for(int indexQuantity=0;indexQuantity<quantity;++indexQuantity)
            {
                AbstractCard* abCard = db.cardById(idCard);

                if(abCard != nullptr)
                {
                    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", abCard.name=" + abCard->name());
                    listCards.append(abCard);
                }
                else
                {
                    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": id unknown (" + QString::number(idCard) + ")");
                }
            }
        }
        else
        {
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": valueCard is not an jsonObject");
        }
    }

    //Check the packet is ok
    if(listCards.count() < MAXCARDS_DECK)
    {
        jsonResponse["success"] = "ko";
        jsonResponse["error"] = "No enough card in packet";
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": No enough card in packet");
    }
    else if(listCards.count() > MAXCARDS_DECK)
    {
        jsonResponse["success"] = "ko";
        jsonResponse["error"] = "Too many cards in packet";
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": Too many cards in packet");
    }
    else
    {
        Player* play = m_gameManager->playerByName(namePlayer);

        if(play != nullptr)
        {
            play->fillDeck(listCards);
            if(m_gameManager->initPlayer(play))
            {
                jsonResponse["success"] = "ok";
                jsonResponse["deck"] = MAXCARDS_DECK;
                jsonResponse["rewards"] = MAXCARDS_REWARD;
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", " + namePlayer + " created");
            }
            else
            {
                play->emptyingDeck();

                jsonResponse["success"] = "ko";
;                jsonResponse["error"] = "No enough base to play";
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": No enough base to play");
            }
        }
        else
        {
            jsonResponse["success"] = "ko";
            jsonResponse["error"] = "Player " + namePlayer + " is nullptr";
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error for " + namePlayer + ": player is nullptr");
        }
    }

    return jsonResponse;
}

QJsonObject Controller::moveACard(const QString &namePlayer, Player::EnumPacket packetOrigin, Player::EnumPacket packetDestination, int indexCardOrigin, int indexCardDestination)
{
    QJsonObject jsonResponse;

    Log::instance()->write(QString(__PRETTY_FUNCTION__));

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

    Log::instance()->write(QString(__PRETTY_FUNCTION__));

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
                jsonResponse["success"] = "ok";
                m_gameManager->endOfTurn();
                break;
            case CardPokemon::Attack_AttackBlocked:
                jsonResponse["success"] = "ko";
                jsonResponse["error"] = "you cannot use this attack for this turn";
                break;
            case CardPokemon::Attack_NotEnoughEnergies:
                jsonResponse["success"] = "ko";
                jsonResponse["error"] = "you don't have enough energies";
                break;
            case CardPokemon::Attack_WrongStatus:
                jsonResponse["success"] = "ko";
                jsonResponse["error"] = "you cannot attack because you are " + pokemonAttacking->statusFormatString();
                break;
            case CardPokemon::Attack_IndexNOK:
                jsonResponse["success"] = "ko";
                jsonResponse["error"] = "error index";
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + " Erreur d'index");
                break;
            case CardPokemon::Attack_UnknownError:
                jsonResponse["success"] = "ko";
                jsonResponse["error"] = "error unknown";
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + " Erreur inconnue");
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
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + " Erreur lors de l'échange");
                jsonResponse["success"] = "ko";
                jsonResponse["error"] = "error during the retreat";
            }
        }
        else
        {
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + " erreur de indexAttack=" + QString::number(indexAttack));
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

QJsonObject Controller::displayPacketResponse(const QJsonDocument &document)
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__));
#endif

    QJsonObject jsonResponse;
    if(m_displayData != nullptr)
    {
        //Check data
        if(m_displayData->messageResponseFromClient(document) == true)
        {
            m_gameManager->endOfSelectionDisplay(m_displayData->argument());
            emit selectionDisplayFinished();
        }
        else
        {
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", the document is incorrect");
        }

        //Send the result to the client whatever is the result
        jsonResponse = m_displayData->messageResultToClient();

        //Finish with the display, wa can delete it
        delete m_displayData;
        m_displayData = nullptr;
    }
    else
    {
        jsonResponse["success"] = "ko";
        jsonResponse["error"] = "no display awaiting";
    }

    return jsonResponse;
}

void Controller::sendNotifNewGameCreated(int uid, const QString &nameGame, const QString& player1, const QString& player2)
{
    NotificationNewGameCreated *notif = new NotificationNewGameCreated(uid,
                                                                       nameGame,
                                                                       player1,
                                                                       player2);

    unsigned int readPoint = m_historicNotif.readPoint();
    m_historicNotif.addNewNotification(notif);

    //construction of the notification to send
    //owner
    QByteArray messageToRespond = player1.toLatin1() + ";";
    QJsonObject objNotifOwner = m_historicNotif.buildJsonNotificationFrom(readPoint, player1);
    if(objNotifOwner != QJsonObject())
    {
        QJsonObject objOwner;
        objOwner["actions"] = objNotifOwner;
        messageToRespond += QJsonDocument(objOwner).toJson(QJsonDocument::Compact) + ";";
    }
    else
    {
        messageToRespond += ";";
    }

    //opponent
    messageToRespond += player2.toLatin1() + ";";
    QJsonObject objNotifOpponent = m_historicNotif.buildJsonNotificationFrom(readPoint, player2);
    if(objNotifOpponent != QJsonObject())
    {
        QJsonObject objOpponent;
        objOpponent["actions"] = objNotifOpponent;
        messageToRespond += QJsonDocument(objOpponent).toJson(QJsonDocument::Compact) + ";";
    }
    else
    {
        messageToRespond += ";";
    }

    //others
    messageToRespond += ";";        //for all others players/viewers
    QJsonObject objNotifOther = m_historicNotif.buildJsonNotificationFrom(readPoint);
    if(objNotifOther != QJsonObject())
    {
        QJsonObject objOther;
        objOther["actions"] = objNotifOther;
        messageToRespond += QJsonDocument(objOther).toJson(QJsonDocument::Compact);
    }

    m_communication->write(messageToRespond);
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

void Controller::sendNotifCardMoved(const QString &namePlayer, ConstantesShared::EnumPacket packetOrigin, int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, int idCard, bool showCardToEveryone)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) +
                ", namePlayer: " + namePlayer +
                ", historic: " + QString::number(m_historicNotif.count()) +
                ", packetOrigin: " + QString::number(packetOrigin) +
                ", indexCardOrigin: " + QString::number(indexCardOrigin) +
                ", packetDestination: " + QString::number(packetDestination) +
                ", idCard: " + QString::number(idCard) +
                ", showCardToEveryone: " + QString(showCardToEveryone ? "true" : "false"));

    AbstractNotification* notif = new NotificationCardMoved(namePlayer,
                                                            idCard,
                                                            packetOrigin,
                                                            indexCardOrigin,
                                                            packetDestination,
                                                            showCardToEveryone);
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
    for(unsigned short i=0;i<pokemon->modelListOfEnergies()->countCard();++i)
    {
        for(int indexQuantity=0;indexQuantity<pokemon->modelListOfEnergies()->energy(i)->quantity();++indexQuantity)
        {
            listEnergies.append(pokemon->modelListOfEnergies()->energy(i)->id());
        }
    }

    AbstractNotification* notif = new NotificationDataPokemonChanged(namePlayer,
                                                                     packet,
                                                                     indexCard,
                                                                     pokemon->lifeLeft(),
                                                                     static_cast<short>(pokemon->status()),
                                                                     mapAttackAvailable,
                                                                     listEnergies);
    m_historicNotif.addNewNotification(notif);
}

void Controller::sendNotifPokemonSwitched(const QString &namePlayer, ConstantesShared::EnumPacket packet, int indexCard, int newIdCard, bool keepEnergy)
{
    AbstractNotification* notif = new NotificationPokemonSwitched(namePlayer,
                                                                  packet,
                                                                  indexCard,
                                                                  newIdCard,
                                                                  keepEnergy);

    m_historicNotif.addNewNotification(notif);
}

void Controller::sendNotifEnergyAdded(const QString &namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, int idEnergy)
{
    AbstractNotification* notif = new NotificationEnergyAdded(namePlayer,
                                                              packetOrigin,
                                                              indexCardOrigin,
                                                              packetDestination,
                                                              indexCardDestination,
                                                              idEnergy);
    m_historicNotif.addNewNotification(notif);
}

void Controller::sendNotifEnergyRemoved(const QString &namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, int indexEnergy)
{
    AbstractNotification* notif = new NotificationEnergyRemoved(namePlayer,
                                                                packetOrigin,
                                                                indexCardOrigin,
                                                                packetDestination,
                                                                indexCardDestination,
                                                                indexEnergy);
    m_historicNotif.addNewNotification(notif);
}

void Controller::sendNotifHeadOrTailDone(const QString &namePlayer, QList<unsigned short> coins)
{
    AbstractNotification* notif = new NotificationHeadOrTail(namePlayer,
                                                             coins);

    m_historicNotif.addNewNotification(notif);
}

void Controller::sendNotifDisplayPacket(const QString &namePlayer, QMap<int, int> cards)
{
    AbstractNotification* notif = new NotificationDisplayPackets(namePlayer,
                                                                 cards);

    m_historicNotif.addNewNotification(notif);
}

QString Controller::nameOfEnemy(const QString &nameCurrentPlayer)
{
    QString namePlayer = "";
    Player* currentPlayer = m_gameManager->playerByName(nameCurrentPlayer);

    if(currentPlayer)
    {
        Player* opponent = m_gameManager->enemyOf(currentPlayer);
        if(opponent)
            namePlayer = opponent->name();
    }

    return namePlayer;
}
