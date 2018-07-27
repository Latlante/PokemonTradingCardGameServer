#ifndef MODELTABLEGAMES_H
#define MODELTABLEGAMES_H

#include <QAbstractTableModel>
#include <QDateTime>

class ModelTableGames : public QAbstractTableModel
{
    Q_OBJECT
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

public:
    explicit ModelTableGames(QObject *parent = nullptr);

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int columnCount(const QModelIndex & = QModelIndex()) const override;
    virtual int rowCount(const QModelIndex & = QModelIndex()) const override;

public slots:
    void addNewGame(unsigned int id, const QString& nameGame, const QString& creator, const QString& opponent);
    void removeAGame(int index);

signals:

private:
    struct InfoGame
    {
        unsigned int id;
        QString nameGame;
        QString playerCreator;
        QString playerOpponent;
        QDateTime dateCreation;
    };
    QList<InfoGame> m_listInfos;
};

#endif // MODELTABLEGAMES_H
