#include "modellistusers.h"

ModelListUsers::ModelListUsers(QObject *parent) :
    QAbstractListModel(parent),
    m_listInfos({})
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QVariant ModelListUsers::data(const QModelIndex &index, int role) const
{
    if((index.isValid()) &&
            (index.row() >= 0) && (index.row() < rowCount()))
    {
        if(role == Qt::DisplayRole)
        {
            const QString nameUser = m_listInfos[index.row()].name;
            if(nameUser != "")
                return nameUser;

            return m_listInfos[index.row()].socketDescriptor;
        }
    }

    return QVariant();
}

int ModelListUsers::rowCount(const QModelIndex&) const
{
    return m_listInfos.count();
}

/************************************************************
*****			  FONCTIONS SLOT PUBLIQUES              *****
************************************************************/
void ModelListUsers::addNewUser(int socketDescriptor, const QString& nameUser)
{
    InfoUser info = { socketDescriptor, nameUser };

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_listInfos.append(info);
    endInsertRows();
}

void ModelListUsers::removeAnUser(int socketDescriptor)
{
    int index = indexListFromSocketDescriptor(socketDescriptor);
    if((index >= 0) && (index < m_listInfos.count()))
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_listInfos.removeAt(index);
        endRemoveRows();
    }
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
int ModelListUsers::indexListFromSocketDescriptor(int socketDescriptor)
{
    int indexFound = -1;
    int indexLoop = 0;

    while((indexFound == -1) && (indexLoop < m_listInfos.count()))
    {
        if(m_listInfos[indexLoop].socketDescriptor == socketDescriptor)
            indexFound = indexLoop;
        else
            indexLoop++;
    }

    return indexFound;
}
