#ifndef MODELTABLEGAMES_H
#define MODELTABLEGAMES_H

#include <QAbstractTableModel>
#include <QDateTime>

class ModelTableGames : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnRole
    {
        ROLE_ID = 0,
        ROLE_NAME,
        ROLE_PLAYER_CREATOR,
        ROLE_PLAYER_OPPONENT,
        ROLE_DATE_CREATION,
        ROLE_BUTTON_DELETE,
        ROLE_COUNT
    };

    explicit ModelTableGames(QObject *parent = nullptr);

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual int columnCount(const QModelIndex & = QModelIndex()) const override;
    virtual int rowCount(const QModelIndex & = QModelIndex()) const override;

public slots:
    void addNewGame(int socketDescriptor);
    void gameAuthentified(int socketDescriptor, unsigned int id, const QString& nameGame, const QString& creator, const QString& opponent);
    void removeAGame(int socketDescriptor);

signals:

private:
    struct InfoGame
    {
        unsigned int id;
        QString nameGame;
        QString playerCreator;
        QString playerOpponent;
        QDateTime dateCreation;
        int socketDescriptor;
    };
    QList<InfoGame> m_listInfos;

    int indexListFromSocketDescriptor(int socketDescriptor);
};

#endif // MODELTABLEGAMES_H
