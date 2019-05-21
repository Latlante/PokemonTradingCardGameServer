#include "authentification.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

const QString Authentification::m_PATH_FILE_USERS = "Files/users.txt";

Authentification::Authentification() :
    m_user(""),
    m_uid(0),
    m_token("")
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
QJsonArray Authentification::listOfAllUsers()
{
    QFile fileUsers(m_PATH_FILE_USERS);
    fileUsers.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = fileUsers.readAll();
    fileUsers.close();

    QJsonArray listUsers;
    QList<QString> contentSplitByLine = content.split("\n");

    foreach(QString line, contentSplitByLine)
    {
        QJsonObject obj;
        obj["uid"] = line.section(';', Position_Uid, Position_Uid);
        obj["name"] = line.section(';', Position_Name, Position_Name);
        listUsers.append(obj);
    }

    return listUsers;
}

QString Authentification::namePlayerFromUid(unsigned int uidPlayer)
{
    QFile fileUsers(m_PATH_FILE_USERS);
    fileUsers.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = fileUsers.readAll();
    fileUsers.close();

    QList<QString> listPlayers = content.split("\n");
    QList<QString>::iterator itListPlayers = listPlayers.begin();
    QString namePlayer = "";

    while((itListPlayers != listPlayers.end()) && (namePlayer == ""))
    {
        const QString infoPlayer = *(itListPlayers);
        if(infoPlayer.section(';', Position_Uid, Position_Uid) == QString::number(uidPlayer))
            namePlayer = infoPlayer.section(';', Position_Name, Position_Name);

        itListPlayers++;
    }

    return namePlayer;
}

unsigned int Authentification::uidPlayerFromName(const QString &name)
{
    QFile fileUsers(m_PATH_FILE_USERS);
    fileUsers.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = fileUsers.readAll();
    fileUsers.close();

    QList<QString> listPlayers = content.split("\n");
    QList<QString>::iterator itListPlayers = listPlayers.begin();
    unsigned int uidPlayer = 0;

    while((itListPlayers != listPlayers.end()) && (uidPlayer == 0))
    {
        const QString infoPlayer = *(itListPlayers);
        if(infoPlayer.section(';', Position_Name, Position_Name) == name)
            uidPlayer = infoPlayer.section(';', Position_Uid, Position_Uid).toUInt();

        itListPlayers++;
    }

    return uidPlayer;
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
bool Authentification::checkUser(QString user, QString password)
{
    bool userFound = false;

    QFile fileUsers(m_PATH_FILE_USERS);
    fileUsers.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = fileUsers.readAll();
    fileUsers.close();

    QList<QString> contentSplitByLine = content.split("\n");

    foreach(QString line, contentSplitByLine)
    {
        if((line.section(';', Position_Name, Position_Name) == user) &&
                (line.section(';', Position_Password, Position_Password) == password))
        {
            userFound = true;
            m_user = user;
            m_uid = static_cast<unsigned int>(line.section(';', Position_Uid, Position_Uid).toInt());
            m_token ="token";
        }
    }

    return userFound;
}

QString Authentification::user() const
{
    return m_user;
}

unsigned int Authentification::uid() const
{
    return m_uid;
}

QString Authentification::token() const
{
    return m_token;
}
