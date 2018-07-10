#ifndef STDLISTENERWRITTER_H
#define STDLISTENERWRITTER_H

#include <QObject>

class StdListenerWritter : public QObject
{
    Q_OBJECT
public:
    explicit StdListenerWritter(QObject *parent = nullptr);

    void startListening();
    void stopListening();

    void write(QByteArray message);

signals:
    void messageReceived(QByteArray);

private:
    QThread m_thread;
    bool m_stopThread;

    void listening();
};

#endif // STDLISTENERWRITTER_H
