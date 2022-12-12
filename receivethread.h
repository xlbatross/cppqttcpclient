#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include <QDebug>
#include "wtcpclient.h"
//#include "ltcpclient.h"
#include "response.h"

class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveThread(
        WTCPClient * client,
//        LTCPClient * client,
        QObject *parent = nullptr
    );
    ~ReceiveThread();

    void initHeaderBytes();
    void initDataBytesList();

    void run() override;


private:
    bool isRunning;

    WTCPClient * client;
//    LTCPClient * client;

    char * headerBytes;
    char ** dataBytesList;
    int headSize;
    std::vector<int> dataLengthList;
    int responseType;

    ResRoomList * resRoomList;
    ResMakeRoom * resMakeRoom;

public slots:
    void start();

signals:
    void disconnectServerSignal();
//    void viewImageSignal(DataHeader * receiveHeader, const char * const * data);
    void resRoomListSignal(ResRoomList *);
    void resMakeRoomSignal(ResMakeRoom *);
};

#endif // RECEIVETHREAD_H
