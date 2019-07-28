#include "notificationnewmessage.h"

NotificationNewMessage::NotificationNewMessage(const QString &namePlayer, const QString &message, bool showToEveryOne) :
    AbstractNotification(ConstantesShared::PHASE_NotifNewMessage, namePlayer),
    m_message(message),
    m_showToEveryOne(showToEveryOne)
{

}

NotificationNewMessage::~NotificationNewMessage()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationNewMessage::messageJsonForOwner()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["message"] = m_message;

    return jsonResponse;
}

QJsonObject NotificationNewMessage::messageJsonForOthers()
{
    QJsonObject jsonResponse = QJsonObject();

    if(m_showToEveryOne == true)
    {
        jsonResponse = initObject();
        jsonResponse["message"] = m_message;
    }

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
