#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include <QDebug>
//#include "wtcpclient.h"
#include "ltcpclient.h"
#include "dataheader.h"

class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveThread(
//        WTCPClient * client,
        LTCPClient * client,
        QObject *parent = nullptr
    );
    ~ReceiveThread();

    void initReceiveDatas();

    void run() override;


private:
//    WTCPClient * client;
    LTCPClient * client;
    DataHeader * receiverHeader;
    int receiveDatasSize;
    char ** receiveDatas;
    bool isRunning;


public slots:
    void start();

signals:
    void viewImageSignal(const char * const * data, const int height, const int width, const int channels);
};

#endif // RECEIVETHREAD_H
