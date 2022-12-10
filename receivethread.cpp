#include "receivethread.h"

ReceiveThread::ReceiveThread(WTCPClient * client, QObject *parent)
    : QThread{parent}
    , client(client)
    , isRunning(false)
{
    this->receiveData = NULL;
    this->receiveDataSize = 0;
}

ReceiveThread::~ReceiveThread()
{

}

void ReceiveThread::initReceiveData()
{
    if (this->receiveData != NULL && receiveDataSize != 0)
    {
        for (int i = 0; i < receiveDataSize; i++)
        {
            if (this->receiveData[i] != NULL)
                delete [] this->receiveData[i];
        }
        delete [] this->receiveData;
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

        this->initReceiveData();
        this->receiveData = new char * [this->receiverHeader->dataCount()]();
        for (int i = 0; i < this->receiverHeader->dataCount(); i++)
        {
            this->client->receiveByteData(&this->receiveData[i]);
        }
        this->receiveDataSize = this->receiverHeader->dataCount();

        switch(this->receiverHeader->responseType())
        {
        case DataHeader::resImage:
            emit viewImageSignal(this->receiveData, this->receiverHeader->attr()[0], this->receiverHeader->attr()[1], this->receiverHeader->attr()[2]);
            break;
        }
    }
}

void ReceiveThread::start()
{
    isRunning = true;
    QThread::start();
}
