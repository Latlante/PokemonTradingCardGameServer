#ifndef MODELLISTUSERS_H
#define MODELLISTUSERS_H

#include <QAbstractListModel>

class ModelListUsers : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ModelListUsers(QObject *parent = nullptr);

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int rowCount(const QModelIndex & = QModelIndex()) const override;

public slots:
    void addNewUser(int socketDescriptor, const QString& nameUser);
    void removeAUser(int socketDescriptor);

private:
    struct InfoUser
    {
        int socketDescriptor;
        QString name;
    };
    QList<InfoUser> m_listInfos;

    int indexListFromSocketDescriptor(int socketDescriptor);
};

#endif // MODELLISTUSERS_H
