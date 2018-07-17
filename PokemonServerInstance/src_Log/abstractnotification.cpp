#include "abstractnotification.h"

AbstractNotification::AbstractNotification(const QString& namePlayer) :
    m_namePlayer(namePlayer)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
const QString AbstractNotification::namePlayer()
{
    return m_namePlayer;
}

void AbstractNotification::setNamePlayer(const QString &namePlayer)
{
    m_namePlayer = namePlayer;
}

QByteArray AbstractNotification::messageJsonComplete()
{
    QJsonObject objOwner = messageJsonForOwner();
    QJsonObject objOthers = messageJsonForOthers();

    return namePlayer() + ";" + QJsonDocument(objOwner).toJson(QJsonDocument::Compact) + ";" + QJsonDocument(objOthers).toJson(QJsonDocument::Compact);
}
