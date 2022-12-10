#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include "wtcpclient.h"
#include "dataheader.h"

class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveThread(WTCPClient * client, QObject *parent = nullptr);
    ~ReceiveThread();

    void initReceiveData();

    void run() override;


private:
    WTCPClient * client;
    DataHeader * receiverHeader;
    char ** receiveData;
    long receiveDataSize;
    bool isRunning;


public slots:
    void start();

signals:
    void viewImageSignal(const char * const * data, const long height, const long width, const long channels);
};

#endif // RECEIVETHREAD_H
