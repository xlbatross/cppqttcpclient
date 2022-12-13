#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include <QDebug>
//#include "wtcpclient.h"
#include "ltcpclient.h"
#include "response.h"

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

    void initHeaderBytes();
    void initDataBytesList();

    void run() override;


private:
    bool isRunning;

//    WTCPClient * client;
    LTCPClient * client;

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
    ResLogin * resLogin;
    ResProImage * resProImage;
    ResFirstImage * resFirstImage;
    ResSecondImage * resSecondImage;
    ResThirdImage * resThirdImage;
    ResForthImage * resForthImage;


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
    void resLoginSignal(ResLogin *);

    void resProImageSignal(ResProImage *);
    void resFirstImageSignal(ResFirstImage *);
    void resSecondImageSignal(ResSecondImage *);
    void resThirdImageSignal(ResThirdImage *);
    void resForthImageSignal(ResForthImage *);
};

#endif // RECEIVETHREAD_H
