#include "instancemanager.h"

#include <QDebug>
#include <QProcess>

const QString InstanceManager::m_PATH_INSTANCE = "D:/Users/sapiensc/Documents/GitHub/PokemonTradingCardGameServer/build-PokemonServerInstance-Desktop_Qt_5_11_0_MinGW_32bit-Release/release/PokemonServerInstance.exe";
InstanceManager* InstanceManager::m_instance = new InstanceManager();
unsigned int InstanceManager::m_indexGame = 1;

InstanceManager::InstanceManager(QObject *parent) :
    QObject(parent)
{

}

InstanceManager::~InstanceManager()
{
    while(m_listGame.count() > 0)
    {
        removeGame(0);
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
unsigned int InstanceManager::createNewGame(int uidPlay1, int uidPlay2, QString name)
{
    qDebug() << "Creation new process";

    //Check the name does not already exist
    unsigned int indexNewGame = 0;
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::readyRead, this, &InstanceManager::onReadyRead_Process);
    process->start(m_PATH_INSTANCE, {name, "Player1", "Player2"});

    if(process->waitForStarted())
    {
        QString nameOfTheGame = name;
        if(nameOfTheGame == "")
            nameOfTheGame = "game #" + QString::number(m_listGame.count());

        InstanceGame game = { process, nameOfTheGame, uidPlay1, uidPlay2 };
        m_listGame.insert(m_indexGame, game);
        indexNewGame = m_indexGame;
        m_indexGame++;
    }
    else
    {
        qCritical() << "Error creation new process";
    }

    return indexNewGame;
}

QProcess* InstanceManager::game(int index)
{
    QProcess* processToReturn = nullptr;

    if((index >= 0) && (m_listGame.contains(index)))
    {
        processToReturn = m_listGame[index].process;
    }

    return processToReturn;
}

bool InstanceManager::removeGame(int index)
{
    bool success = false;
    if(m_listGame.contains(index) == true)
    {
        InstanceGame game = m_listGame.take(index);
        QProcess* process = game.process;

        if(process != nullptr)
        {
            process->close();
            process->deleteLater();
            success = true;
        }
    }

    return success;
}

bool InstanceManager::write(unsigned int uidGame, QByteArray message)
{
    qint64 byteWritten = 0;

    if((m_listGame.contains(uidGame)) && (!message.isEmpty()))
    {
        QProcess* process = m_listGame[uidGame].process;
        byteWritten = process->write(message + "\n");
    }

    return message.length() == byteWritten;
}

QString InstanceManager::nameOfTheGameFromUidGame(int uidGame)
{
    QString name = "";

    if((uidGame >= 0) && (m_listGame.contains(uidGame)))
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

QList<unsigned int> InstanceManager::listUidGamesFromUidPlayer(int uidPlayer)
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

/************************************************************
*****			  FONCTIONS SLOT PRIVEES				*****
************************************************************/
void InstanceManager::onReadyRead_Process()
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    unsigned int uidGame = uidGameFromQProcess(process);
    QByteArray message = process->readLine();

    emit readyRead(uidGame, message);
}
