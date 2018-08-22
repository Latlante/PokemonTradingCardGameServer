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
    QAbstractTableModel(parent)
{
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
}

InstanceManager::~InstanceManager()
{
    while(m_listGames.count() > 0)
    {
        removeGame(data(index(0, ROLE_ID)).toUInt());
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
        process->start(m_PATH_INSTANCE, {QString::number(m_indexGame), name, QString::number(uidPlayCreator), namePlayerCreator, QString::number(uidPlayOpponent), namePlayerOpponent});

        if(process->waitForStarted())
		{
			QString nameOfTheGame = name;
			if(nameOfTheGame == "")
                nameOfTheGame = "game #" + QString::number(m_listGames.count());

            InstanceGame game = { m_indexGame,
                                  false,
                                  process,
                                  nameOfTheGame,
                                  uidPlayCreator,
                                  uidPlayOpponent,
                                  QDateTime::currentDateTime()
                                };
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_listGames.append(game);
            endInsertRows();

			indexNewGame = m_indexGame;
			m_indexGame++;

			sendNotifNewGameCreated(indexNewGame, uidPlayCreator, uidPlayOpponent);
            /*emit newGameCreated(indexNewGame,
                                name,
                                Authentification::namePlayerFromUid(uidPlayCreator),
                                Authentification::namePlayerFromUid(uidPlayOpponent));*/
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

QProcess* InstanceManager::game(unsigned int uidGame)
{
    QProcess* processToReturn = nullptr;
    int indexList = indexOfUidGame(uidGame);

    if((indexList >= 0) && (indexList < m_listGames.count()))
    {
        processToReturn = m_listGames[indexList].process;
    }

    return processToReturn;
}

bool InstanceManager::removeGame(unsigned int uidGame)
{
    qDebug() << __PRETTY_FUNCTION__;
    bool success = false;
    int indexList = indexOfUidGame(uidGame);

    if((indexList >= 0) && (indexList < m_listGames.count()))
    {
        InstanceGame game = m_listGames.takeAt(indexList);
        QProcess* process = game.process;

        if(process != nullptr)
        {
            //process->write("exit");
            process->close();
            if(process->waitForFinished(10000))
            {
                process->deleteLater();
                emit gameRemoved(static_cast<int>(uidGame));
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
    int index = 0;
    bool nameFound = false;

    while((index < m_listGames.count()) && (nameFound == false))
    {
        if(nameGame == m_listGames[index].nameGame)
            nameFound = true;

        index++;
    }

    return !nameFound;
}

QString InstanceManager::nameOfTheGameFromUidGame(unsigned int uidGame)
{
    QString name = "";
    int index = indexOfUidGame(uidGame);

    if((index >= 0) && (index < m_listGames.count()))
        name = m_listGames[index].nameGame;

    return name;
}

unsigned int InstanceManager::uidGameFromQProcess(QProcess *process)
{
    unsigned int uidGame = 0;
    int index = 0;

    while((index < m_listGames.count()) && (uidGame == 0))
    {
        if(process == m_listGames[index].process)
            uidGame = m_listGames[index].uid;

        index++;
    }

    return uidGame;
}

QVariant InstanceManager::data(const QModelIndex &index, int role) const
{
    if((index.isValid()) &&
            (index.row() >= 0) && (index.row() < rowCount()) &&
            (index.column() >= 0) && (index.column() < columnCount()))
    {
        if(role == Qt::DisplayRole)
        {
            switch(index.column())
            {
            case ROLE_ID:               return m_listGames[index.row()].uid;
            case ROLE_NAME:             return m_listGames[index.row()].nameGame;
            case ROLE_PLAYER_CREATOR:
            {
                unsigned int uidPlayer = m_listGames[index.row()].uidPlayer1;
                const QString namePlayer = Authentification::namePlayerFromUid(uidPlayer);
                return namePlayer + " (" + QString::number(uidPlayer) + ")";
            }
            case ROLE_PLAYER_OPPONENT:
            {
                unsigned int uidPlayer = m_listGames[index.row()].uidPlayer2;
                const QString namePlayer = Authentification::namePlayerFromUid(uidPlayer);
                return namePlayer + " (" + QString::number(uidPlayer) + ")";
            }
            case ROLE_DATE_CREATION:    return m_listGames[index.row()].dateCreation.toString("dd/MM - hh:mm");
            case ROLE_BUTTON_DELETE:    return "X";
            }
        }
    }

    return QVariant();
}

int InstanceManager::columnCount(const QModelIndex&) const
{
    return static_cast<int>(ROLE_COUNT);
}

int InstanceManager::rowCount(const QModelIndex&) const
{
    return m_listGames.count();
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

    foreach(InstanceGame instance, m_listGames)
    {
        if((instance.uidPlayer1 == uidPlayer) ||
                (instance.uidPlayer2 == uidPlayer))
        {
            listUidGames.append(instance.uid);
        }
    }

    return listUidGames;
}

QList<unsigned int> InstanceManager::listUidPlayersFromUidGame(unsigned int uidGame)
{
    QList<unsigned int> listPlayers;
    int index = indexOfUidGame(uidGame);

    if((index >= 0) && (index < m_listGames.count()))
    {
        listPlayers.append(m_listGames[index].uidPlayer1);
        listPlayers.append(m_listGames[index].uidPlayer2);
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
int InstanceManager::indexOfUidGame(unsigned int uidGame)
{
    int indexLoop = 0;
    int indexList = -1;

    while((indexLoop < m_listGames.count()) && (indexList == -1))
    {
        if(uidGame == m_listGames[indexLoop].uid)
            indexList = indexLoop;

        indexLoop++;
    }

    return indexList;
}

void InstanceManager::sendNotifNewGameCreated(unsigned int uidGame, unsigned int uidPlayerCreator, unsigned int uidPlayerOpponent)
{
    QJsonObject jsonResponse;
    const QString namePlayerCreator = Authentification::namePlayerFromUid(uidPlayerCreator);
    const QString namePlayerOpponent = Authentification::namePlayerFromUid(uidPlayerOpponent);

    QJsonObject jsonAction;
    jsonAction["phase"] = ConstantesShared::PHASE_NotifNewGameCreated;
    jsonAction["uidGame"] = static_cast<int>(uidGame);
    jsonAction["nameGame"] = nameOfTheGameFromUidGame(uidGame);
    jsonAction["opponent"] = namePlayerCreator;

    jsonResponse["indexBegin"] = 1;
    jsonResponse["indexEnd"] = 2;
    jsonResponse["1"] = jsonAction;

    QByteArray messageJson = namePlayerOpponent.toLatin1() + ";" + QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact);
    emit readyRead(uidGame, messageJson);
}
