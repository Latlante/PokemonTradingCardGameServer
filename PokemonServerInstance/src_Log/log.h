#ifndef LOG_H
#define LOG_H

#include <QObject>

class QFile;

class Log
{
public:
    explicit Log(QString name);
    ~Log();

    void write(QString message);

signals:

private:
    QFile* m_file;
};

#endif // LOG_H
