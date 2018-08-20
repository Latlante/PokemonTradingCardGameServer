#include "log.h"

#include <QDateTime>
#include <QFile>

Log* Log::m_instance = nullptr;

Log::Log(const QString &name) :
    m_file(new QFile("Logs/log_" + name + ".txt"))
{
}

Log::~Log()
{
    delete m_file;
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
Log* Log::createInstance(const QString &nameFile)
{
    if(m_instance == nullptr)
    {
        m_instance = new Log(nameFile);
    }

    return m_instance;
}

void Log::deleteInstance()
{
    if(m_instance != nullptr)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

Log* Log::instance()
{
    return m_instance;
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
