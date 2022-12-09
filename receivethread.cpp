#include "receivethread.h"

ReceiveThread::ReceiveThread(WTCPClient * client, QObject *parent)
    : QThread{parent}
    , client(client)
    , isRunning(false)
{
    this->receiveData = NULL;
}

void ReceiveThread::run()
{
    while (this->isRunning)
    {
        this->receiverHeader = this->client->receiveHeader();

        if (!this->receiverHeader->decode()
         || !this->client->receiveByteData(&this->receiveData))
        {
            this->isRunning = false;
            break;
        }

        switch(this->receiverHeader->dataType())
        {
        case DataHeader::Image:
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
