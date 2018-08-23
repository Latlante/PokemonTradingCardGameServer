#include "abstractdisplaydata.h"

AbstractDisplayData::AbstractDisplayData(const QString &namePlayer) :
    QObject(),
    m_namePlayer(namePlayer),
    m_success(false),
    m_error("")
{

}

AbstractDisplayData::~AbstractDisplayData()
{

}

/************************************************************
*****               FONCTIONS PUBLIQUES					*****
************************************************************/
bool AbstractDisplayData::success()
{
    return m_success;
}

QString AbstractDisplayData::error() const
{
    return m_error;
}

QVariant AbstractDisplayData::argument() const
{
    return m_argument;
}
