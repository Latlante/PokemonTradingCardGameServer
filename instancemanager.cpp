#include "instancemanager.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "authentification.h"
#include "Share/constantesshared.h"

const QString InstanceManager::m_PATH_INSTANCE = "C:/Users/Corentin/Documents/Programmes/PokemonTradingCardGameServer/build-PokemonServerInstance-Desktop_Qt_5_11_0_MinGW_32bit3-Release/release/PokemonServerInstance.exe";
InstanceManager* InstanceManager::m_instance = new InstanceManager();
unsigned int InstanceManager::m_indexGame = 1;

InstanceManager::InstanceManager(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
}

InstanceManager::~InstanceManager()
{
    while(m_listGame.count() > 0)
    {
        removeGame(m_listGame.keys().first());
    }
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
InstanceManager* InstanceManager::instance()
{
    return m_instance;
}

void InstanceManager::deleteInstance()
{
    delete m_instance;
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
bool InstanceManager::checkInstanceExeExists()
{
    return QFile::exists(m_PATH_INSTANCE);
}

unsigned int InstanceManager::createNewGame(unsigned int uidPlayCreator, unsigned int uidPlayOpponent, const QString& name, QString& error)
{
    qDebug() << "Creation new process";
    unsigned int indexNewGame = 0;

	if(uidPlayCreator != uidPlayOpponent)
	{
		QProcess* process = new QProcess();
        const QString namePlayerCreator = Authentification::namePlayerFromUid(uidPlayCreator);
        const QString namePlayerOpponent = Authentification::namePlayerFromUid(uidPlayOpponent);
		connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &InstanceManager::onFinished_Process);
        process->start(m_PATH_INSTANCE, {name, namePlayerCreator, namePlayerOpponent});

        if(process->waitForStarted())
		{
			QString nameOfTheGame = name;
			if(nameOfTheGame == "")
				nameOfTheGame = "game #" + QString::number(m_listGame.count());

			InstanceGame game = { process, nameOfTheGame, uidPlayCreator, uidPlayOpponent };
			m_listGame.insert(m_indexGame, game);
			indexNewGame = m_indexGame;
			m_indexGame++;

			sendNotifNewGameCreated(indexNewGame, uidPlayCreator, uidPlayOpponent);
            emit newGameCreated(indexNewGame,
                                name,
                                Authentification::namePlayerFromUid(uidPlayCreator),
                                Authentification::namePlayerFromUid(uidPlayOpponent));
			qDebug() << "New process started";
		}
		else
		{
			error = "Process does not start";
			qCritical() << "Process does not start";
		}
	}
	else
	{
		error = "You cannot create a game against yourself";
		qCritical() << "You cannot create a game against yourself";
	}

    return indexNewGame;
}

QProcess* InstanceManager::game(unsigned int index)
{
    QProcess* processToReturn = nullptr;

    if(m_listGame.contains(index))
    {
        processToReturn = m_listGame[index].process;
    }

    return processToReturn;
}

bool InstanceManager::removeGame(unsigned int index)
{
    qDebug() << __PRETTY_FUNCTION__;
    bool success = false;
    if(m_listGame.contains(index) == true)
    {
        InstanceGame game = m_listGame.take(index);
        QProcess* process = game.process;

        if(process != nullptr)
        {
            process->write("exit");
            //if(process->waitForFinished(10000))
            {
                process->deleteLater();
                emit gameRemoved(static_cast<int>(index));
                success = true;
            }
            /*else
                qDebug() << __PRETTY_FUNCTION__ << "timeOut to finish the process";*/
        }
    }

    return success;
}

bool InstanceManager::checkNameOfGameIsAvailable(const QString &nameGame)
{
    QList<unsigned int> listKeysGames = m_listGame.keys();
    int index = 0;
    bool nameFound = false;

    while((index < listKeysGames.count()) && (nameFound == false))
    {
        InstanceGame inst = m_listGame[listKeysGames[index]];

        if(nameGame == inst.name)
            nameFound = true;

        index++;
    }

    return !nameFound;
}

QString InstanceManager::nameOfTheGameFromUidGame(unsigned int uidGame)
{
    QString name = "";

    if(m_listGame.contains(uidGame))
        name = m_listGame[uidGame].name;

    return name;
}

unsigned int InstanceManager::uidGameFromQProcess(QProcess *process)
{
    unsigned int uidGame = 0;

    for(QMap<unsigned int, InstanceGame>::iterator it=m_listGame.begin();
        it != m_listGame.end();
        ++it)
    {
        InstanceGame instance = it.value();
        if(instance.process == process)
        {
            uidGame = it.key();
            break;
        }
    }

    return uidGame;
}

bool InstanceManager::isInTheGame(unsigned int uidGame, unsigned int uidPlayer)
{
    QList<unsigned int> listUidGameForPlayer = listUidGamesFromUidPlayer(uidPlayer);

    return listUidGameForPlayer.contains(uidGame);
}

unsigned int InstanceManager::uidOpponentOfInGame(unsigned int uidGame, unsigned int uidPlayer)
{
    unsigned int uidOpponent = 0;
    const QList<unsigned int> listPlayers = listUidPlayersFromUidGame(uidGame);

    if(listPlayers.count() >= 2)
    {
        uidOpponent = (uidPlayer == listPlayers[0]) ? listPlayers[1] : listPlayers[0];
    }

    return uidOpponent;
}

QString InstanceManager::nameOpponentOfInGame(unsigned int uidGame, unsigned int uidPlayer)
{
    QString nameOpponent = "";
    unsigned int uidOpponent = uidOpponentOfInGame(uidGame, uidPlayer);

    if(uidOpponent > 0)
    {
        nameOpponent = Authentification::namePlayerFromUid(uidOpponent);
    }

    return nameOpponent;
}

QList<unsigned int> InstanceManager::listUidGamesFromUidPlayer(unsigned int uidPlayer)
{
    QList<unsigned int> listUidGames;

    for(QMap<unsigned int, InstanceGame>::iterator it=m_listGame.begin();
        it != m_listGame.end();
        ++it)
    {
        InstanceGame instance = it.value();
        if((instance.m_uidPlayer1 == uidPlayer) ||
                (instance.m_uidPlayer2 == uidPlayer))
        {
            listUidGames.append(it.key());
        }
    }

    return listUidGames;
}

QList<unsigned int> InstanceManager::listUidPlayersFromUidGame(unsigned int uidGame)
{
    QList<unsigned int> listPlayers;

    if(m_listGame.contains(uidGame))
    {
        listPlayers.append(m_listGame[uidGame].m_uidPlayer1);
        listPlayers.append(m_listGame[uidGame].m_uidPlayer2);
    }

    return listPlayers;
}

/************************************************************
*****			  FONCTIONS SLOT PRIVEES				*****
************************************************************/
void InstanceManager::onFinished_Process(int exitCode)
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    unsigned int uidGame = uidGameFromQProcess(process);
    qDebug() << __PRETTY_FUNCTION__ << ", the instance " << uidGame << " is closed with code " << exitCode;

    removeGame(uidGame);
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
void InstanceManager::sendNotifNewGameCreated(unsigned int uidGame, unsigned int uidPlayerCreator, unsigned int uidPlayerOpponent)
{
    QJsonObject jsonResponse;
    const QString namePlayerCreator = Authentification::namePlayerFromUid(uidPlayerCreator);
    const QString namePlayerOpponent = Authentification::namePlayerFromUid(uidPlayerOpponent);

    QJsonObject jsonAction;
    jsonAction["phase"] = ConstantesShared::PHASE_NotifNewGameCreated;
    jsonAction["uidGame"] = static_cast<int>(uidGame);
    jsonAction["nameGame"] = m_listGame[uidGame].name;
    jsonAction["opponent"] = namePlayerCreator;

    jsonResponse["indexBegin"] = 1;
    jsonResponse["indexEnd"] = 2;
    jsonResponse["1"] = jsonAction;

    QByteArray messageJson = namePlayerOpponent.toLatin1() + ";" + QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact);
    emit readyRead(uidGame, messageJson);
}
