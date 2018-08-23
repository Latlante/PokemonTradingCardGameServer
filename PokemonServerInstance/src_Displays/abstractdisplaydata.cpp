#include "abstractdisplaydata.h"

AbstractDisplayData::AbstractDisplayData(QObject *parent) :
    QObject(parent),
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
