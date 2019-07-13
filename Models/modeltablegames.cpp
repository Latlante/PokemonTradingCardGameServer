#include "modeltablegames.h"

ModelTableGames::ModelTableGames(QObject *parent) :
    QAbstractTableModel(parent),
    m_listInfos({})
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QVariant ModelTableGames::data(const QModelIndex &index, int role) const
{
    if((index.isValid()) &&
            (index.row() >= 0) && (index.row() < rowCount()) &&
            (index.column() >= 0) && (index.column() < columnCount()))
    {
        if(role == Qt::DisplayRole)
        {
            switch(index.column())
            {
            case ROLE_ID:               return m_listInfos[index.row()].id;
            case ROLE_NAME:             return m_listInfos[index.row()].nameGame;
            case ROLE_PLAYER_CREATOR:   return m_listInfos[index.row()].playerCreator;
            case ROLE_PLAYER_OPPONENT:  return m_listInfos[index.row()].playerOpponent;
            case ROLE_DATE_CREATION:    return m_listInfos[index.row()].dateCreation.toString("dd/MM - hh:mm");
            case ROLE_BUTTON_DELETE:    return "X";
            }
        }
    }

    return QVariant();
}

int ModelTableGames::columnCount(const QModelIndex&) const
{
    return static_cast<int>(ROLE_COUNT);
}

int ModelTableGames::rowCount(const QModelIndex&) const
{
    return m_listInfos.count();
}

/************************************************************
*****			  FONCTIONS SLOT PUBLIQUES              *****
************************************************************/
void ModelTableGames::addNewGame(int socketDescriptor)
{
    InfoGame info = { 0, "", "", "", QDateTime::currentDateTime(), socketDescriptor };

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_listInfos.append(info);
    endInsertRows();
}

void ModelTableGames::gameAuthentified(int socketDescriptor, unsigned int id, const QString& nameGame, const QString& creator, const QString& opponent)
{
    int indexList = indexListFromSocketDescriptor(socketDescriptor);

    if(indexList >= 0)
    {
        InfoGame info = m_listInfos[indexList];
        info.id = id;
        info.nameGame = nameGame;
        info.playerCreator = creator;
        info.playerOpponent = opponent;
        m_listInfos.replace(indexList, info);

        emit dataChanged(index(indexList, 0), index(indexList, ROLE_COUNT));
    }
}

void ModelTableGames::removeAGame(int socketDescriptor)
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
int ModelTableGames::indexListFromSocketDescriptor(int socketDescriptor)
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
