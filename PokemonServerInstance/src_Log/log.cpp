#include "log.h"

#include <QDateTime>
#include <QFile>

Log::Log(QString name) :
    m_file(new QFile("Logs/log_" + name + ".txt"))
{
}

Log::~Log()
{
    delete m_file;
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void Log::write(QString message)
{
    if(m_file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QString textToWrite = QDateTime::currentDateTime().toString("dd/MM - HH:mm:ss.zzz") + ": " + message + "\n";
        m_file->write(textToWrite.toLatin1());
        m_file->close();
    }
}
