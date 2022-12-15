#include "receivethread.h"
#include "response.h"


ReceiveThread::ReceiveThread(
        WTCPClient * client,
//        LTCPClient * client,
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
    , resDisjoinRoom(NULL)
    , resLogin(NULL)
    , resSignUp(NULL)
    , resProImage(NULL)
    , resFirstImage(NULL)
    , resSecondImage(NULL)
    , resThirdImage(NULL)
    , resForthImage(NULL)
    , resChat(NULL)
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
        else if (this->headSize == -2)
        {
            continue;
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
            qDebug() << "response Enter Room";
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
        case Response::DisjoinRoom:
            qDebug() << "response disjoin Room";
            if (resDisjoinRoom != NULL)
                delete resDisjoinRoom;
            resDisjoinRoom = new ResDisjoinRoom(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resDisjoinRoomSignal(resDisjoinRoom);
            break;
        case Response::Login:
            qDebug() << "response Login";
            if (resLogin != NULL)
                delete resLogin;
            resLogin = new ResLogin(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resLoginSignal(resLogin);
            break;
        case Response::SignUp: //####
            qDebug() << "response SignUp";
            if (resSignUp != NULL)
                delete resSignUp;
            resSignUp = new ResSignUp(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resSignUpSignal(resSignUp);
            break;
        case Response::ProImage:
            qDebug() << "response pro Image";
            if (resProImage != NULL)
                delete resProImage;
            resProImage = new ResProImage(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resImageSignal(resProImage);
            break;
        case Response::FirstImage:
            qDebug() << "response first Image";
            if (resFirstImage != NULL)
                delete resFirstImage;
            resFirstImage = new ResFirstImage(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resImageSignal(resFirstImage);
            break;
        case Response::SecondImage:
            qDebug() << "response second Image";
            if (resSecondImage != NULL)
                delete resSecondImage;
            resSecondImage = new ResSecondImage(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resImageSignal(resSecondImage);
            break;
        case Response::ThirdImage:
            qDebug() << "response third Image";
            if (resThirdImage != NULL)
                delete resThirdImage;
            resThirdImage = new ResThirdImage(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resImageSignal(resThirdImage);
            break;
        case Response::ForthImage:
            qDebug() << "response forth Image";
            if (resForthImage != NULL)
                delete resForthImage;
            resForthImage = new ResForthImage(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resImageSignal(resForthImage);
            break;
        //######
        case Response::Chat:
            qDebug() << "response Chat";
            if (resChat != NULL)
                delete resChat;
            resChat = new ResChat(this->headerBytes, this->dataBytesList, this->headSize, this->dataLengthList);
            emit resChatSignal(resChat);
            break;
        }
    }
}

void ReceiveThread::start()
{
    isRunning = true;
    QThread::start();
}
