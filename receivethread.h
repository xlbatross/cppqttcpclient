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

    ResImage * resImage;
    ResRoomList * resRoomList;
    ResMakeRoom * resMakeRoom;
    ResEnterRoom * resEnterRoom;
    ResJoinRoom * resJoinRoom;
    ResDisjoinRoom * resDisjoinRoom;
<<<<<<< HEAD
    ResLogin * resLogin;
=======
>>>>>>> eb5fbce6a906683b9992b73f22f876f3e0feff9b

public slots:
    void start();

signals:
    void disconnectServerSignal();
    void resImageSignal(ResImage *);
    void resRoomListSignal(ResRoomList *);
    void resMakeRoomSignal(ResMakeRoom *);
    void resEnterRoomSignal(ResEnterRoom *);
    void resJoinRoomSignal(ResJoinRoom *);
    void resDisjoinRoomSignal(ResDisjoinRoom *);
<<<<<<< HEAD
    void resLoginSignal(ResLogin *);
=======
>>>>>>> eb5fbce6a906683b9992b73f22f876f3e0feff9b
};

#endif // RECEIVETHREAD_H
