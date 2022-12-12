#include "receivethread.h"
#include "response.h"


ReceiveThread::ReceiveThread(
//        WTCPClient * client,
        LTCPClient * client,
        QObject *parent
    )
    : QThread{parent}
    , isRunning(false)
    , client(client)
    , headerBytes(NULL)
    , dataBytesList(NULL)
    , headSize(0)
    , responseType(0)
    , resImage(NULL)
    , resRoomList(NULL)
    , resMakeRoom(NULL)
    , resEnterRoom(NULL)
    , resJoinRoom(NULL)
{
    this->dataLengthList.resize(0);
}

ReceiveThread::~ReceiveThread()
{
    initHeaderBytes();
    initDataBytesList();
}

void ReceiveThread::initHeaderBytes()
{
    if (this->headerBytes != NULL)
    {
        delete [] this->headerBytes;
        this->headerBytes = NULL;
    }
}

void ReceiveThread::initDataBytesList()
{
    if (this->dataBytesList != NULL && this->dataLengthList.size() != 0)
    {
        for (int i = 0; i < this->dataLengthList.size(); i++)
        {
            if (this->dataBytesList[i] != NULL)
                delete [] this->dataBytesList[i];
        }
        delete [] this->dataBytesList;
        this->dataBytesList = NULL;
        this->dataLengthList.resize(0);
    }
}

void ReceiveThread::run()
{
    while (this->isRunning)
    {
        initHeaderBytes();
        initDataBytesList();

        this->headSize = this->client->receive(&this->headerBytes, &this->dataBytesList, dataLengthList);
        if (this->headSize == -1)
        {
            this->isRunning = false;
            emit disconnectServerSignal();
            break;
        }

        memcpy(&this->responseType, this->headerBytes + sizeof(int) * 1, sizeof(int));

        switch(this->responseType)
        {
        case Response::Image:
            qDebug() << "response Image";
            if (resImage != NULL)
                delete resImage;
            resImage = new ResImage(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resImageSignal(resImage);
            break;
        case Response::RoomList:
            qDebug() << "response Room List";
            if (resRoomList != NULL)
                delete resRoomList;
            resRoomList = new ResRoomList(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resRoomListSignal(resRoomList);
            break;
        case Response::MakeRoom:
            qDebug() << "response Make Room";
            if (resMakeRoom != NULL)
                delete resMakeRoom;
            resMakeRoom = new ResMakeRoom(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resMakeRoomSignal(resMakeRoom);
            break;
        case Response::EnterRoom:
            qDebug() << "response Make Room";
            if (resEnterRoom != NULL)
                delete resEnterRoom;
            resEnterRoom = new ResEnterRoom(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resEnterRoomSignal(resEnterRoom);
            break;
        case Response::JoinRoom:
            qDebug() << "response Join Room";
            if (resJoinRoom != NULL)
                delete resJoinRoom;
            resJoinRoom = new ResJoinRoom(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resJoinRoomSignal(resJoinRoom);
            break;
        }
    }
}

void ReceiveThread::start()
{
    isRunning = true;
    QThread::start();
}
