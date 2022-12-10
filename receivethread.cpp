#include "receivethread.h"

ReceiveThread::ReceiveThread(
//        WTCPClient * client,
        LTCPClient * client,
        QObject *parent
    )
    : QThread{parent}
    , client(client)
    , isRunning(false)
{
    this->receiveDatasSize = 0;
    this->receiveDatas = NULL;
}

ReceiveThread::~ReceiveThread()
{

}

void ReceiveThread::initReceiveDatas()
{
    if (this->receiveDatas != NULL && receiveDatasSize != 0)
    {
        for (int i = 0; i < receiveDatasSize; i++)
        {
            if (this->receiveDatas[i] != NULL)
                delete [] this->receiveDatas[i];
        }
        delete [] this->receiveDatas;
    }
}

void ReceiveThread::run()
{
    while (this->isRunning)
    {
        this->receiverHeader = this->client->receiveHeader();

        if (!this->receiverHeader->decode())
        {
            this->isRunning = false;
            break;
        }

        this->initReceiveDatas();
        this->receiveDatas = new char * [this->receiverHeader->dataCount()]();
        for (int i = 0; i < this->receiverHeader->dataCount(); i++)
        {
            this->client->receiveByteData(&this->receiveDatas[i]);
        }
        this->receiveDatasSize = this->receiverHeader->dataCount();

        switch(this->receiverHeader->responseType())
        {
        case DataHeader::resImage:
            qDebug() << "response Image";
            emit viewImageSignal(this->receiveDatas, this->receiverHeader->attr()[0], this->receiverHeader->attr()[1], this->receiverHeader->attr()[2]);
            break;
        case DataHeader::resRoomList:
            qDebug() << "response Room List";
            break;
        }
    }
}

void ReceiveThread::start()
{
    isRunning = true;
    QThread::start();
}
