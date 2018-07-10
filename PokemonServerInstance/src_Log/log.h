#ifndef LOG_H
#define LOG_H

#include <QObject>

#include <QFile>

class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QString name, QObject *parent = nullptr);

    void write(QString message);

signals:

private:
    QFile m_file;
};

#endif // LOG_H
