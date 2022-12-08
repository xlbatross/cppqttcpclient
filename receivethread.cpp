#include "receivethread.h"

ReceiveThread::ReceiveThread(Client * client, QObject *parent)
    : QThread{parent}
    , client(client)
    , isRunning(false)
{

}

void ReceiveThread::run()
{
    while (isRunning)
    {
        this->headerByteArray.clear();
        this->receiverByteArray.clear();

        if (!this->client->receiveData(this->headerByteArray) || !this->client->receiveData(this->receiverByteArray))
        {
            isRunning = false;
            break;
        }



    }
}

void ReceiveThread::start()
{
    isRunning = true;
    QThread::start();
}
