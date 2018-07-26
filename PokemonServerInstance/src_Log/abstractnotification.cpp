#include "abstractnotification.h"

AbstractNotification::AbstractNotification(ConstantesShared::GamePhase phase, const QString& namePlayer) :
    m_phase(phase),
    m_namePlayer(namePlayer),
    m_indexAction(0)
{

}

AbstractNotification::~AbstractNotification()
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

unsigned int AbstractNotification::indexAction()
{
    return m_indexAction;
}

void AbstractNotification::setIndexAction(unsigned int indexAction)
{
    m_indexAction = indexAction;
}

QByteArray AbstractNotification::messageJsonComplete()
{
    QJsonObject objOwner = messageJsonForOwner();
    QJsonObject objOthers = messageJsonForOthers();

    return namePlayer().toLatin1() + ";" +
            QJsonDocument(objOwner).toJson(QJsonDocument::Compact) + ";" +
            QJsonDocument(objOthers).toJson(QJsonDocument::Compact);
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QJsonObject AbstractNotification::initObject()
{
    QJsonObject obj;

    //obj["indexAction"] = static_cast<int>(indexAction());
    obj["phase"] = static_cast<int>(m_phase);

    if(namePlayer() != "")
        obj["namePlayer"] = namePlayer();

    return obj;
}
