#include "controller.h"

#include "gamemanager.h"
#include "src_Communication/stdlistenerwritter.h"
#include "src_Log/log.h"

Controller::Controller(const QString &nameGame, const QString &player1, const QString &player2, QObject *parent) :
    QObject(parent),
    m_communication(new StdListenerWritter()),
    m_gameManager(GameManager::createInstance()),
    m_log(Log(nameGame))
{
    m_log.write("Creation of the game");
    m_log.write("Players: " + player1 + " versus " + player2);
}
