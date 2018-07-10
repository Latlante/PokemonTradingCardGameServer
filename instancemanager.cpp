#include "instancemanager.h"

#include <QDebug>
#include <QProcess>

InstanceManager* InstanceManager::m_instance = new InstanceManager();

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
void InstanceManager::createNewGame(int uidPlay1, int uidPlay2, QString name)
{
    qDebug() << "Creation new process";

    //Check the name does not already exist

    QProcess* process = new QProcess(this);
    process->start("D:/Users/sapiensc/Documents/build-InstanceTest-Desktop_Qt_5_11_0_MinGW_32bit-Release/release/InstanceTest.exe");

    if(process->waitForStarted())
    {
        QString nameOfTheGame = name;
        if(nameOfTheGame == "")
            nameOfTheGame = "game #" + QString::number(m_listGame.count());

        InstanceGame game = { process, nameOfTheGame, uidPlay1, uidPlay2 };
        m_listGame.append(game);
    }
    else
    {
        qCritical() << "Error creation new process";
    }
}

QProcess* InstanceManager::game(int index)
{
    QProcess* processToReturn = nullptr;

    if((index >= 0) && (index < m_listGame.size()))
    {
        processToReturn = m_listGame.at(index).process;
    }

    return processToReturn;
}

void InstanceManager::removeGame(int index)
{
    if((index >= 0) && (index < m_listGame.size()))
    {
        InstanceGame game = m_listGame.takeAt(index);
        QProcess* process = game.process;

        if(process != nullptr)
        {
            process->close();
            process->deleteLater();
        }
    }
}

QString InstanceManager::nameOfTheGameFromUidGame(int uidGame)
{
    QString name = "";

    if((uidGame >= 0) && (uidGame < m_listGame.count()))
        name = m_listGame[uidGame].name;

    return name;
}

QList<int> InstanceManager::listUidGamesFromUidPlayer(int uidPlayer)
{
    QList<int> listUidGames;

    for(int i=0;i<m_listGame.count();++i)
    {
        if((m_listGame[i].m_uidPlayer1 == uidPlayer) ||
                (m_listGame[i].m_uidPlayer2 == uidPlayer))
        {
            listUidGames.append(i);
        }
    }

    return listUidGames;
}
