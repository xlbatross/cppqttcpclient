#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include "client.h"

class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveThread(Client * client, QObject *parent = nullptr);

    void run() override;


private:
    Client * client;
    bool isRunning;
    QByteArray headerByteArray;
    QByteArray receiverByteArray;

public slots:
    void start();


};

#endif // RECEIVETHREAD_H
