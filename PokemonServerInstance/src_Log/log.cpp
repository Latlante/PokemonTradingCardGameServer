#include "log.h"

#include <QDateTime>

Log::Log(QString name, QObject *parent) :
    QObject(parent),
    m_file(QFile("Logs/log_" + name + ".txt"))
{
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void Log::write(QString message)
{
    if(m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        m_file.write(QDateTime::currentDateTime().toString("dd/MM - HH:mm:ss.zzz") + ": " + message);
        m_file.close();
    }
}
