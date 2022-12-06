#include "client.h"

Client::Client(QObject * parent)

{
    this->sock = new QTcpSocket((QObject *) parent);
}

Client::~Client()
{
    delete this->sock;
}

bool Client::connectToHost(QString serverIp, quint16 portNum)
{
    this->sock->connectToHost(serverIp, portNum);
    return this->sock->waitForConnected();
}

bool Client::disconnectToHost()
{
    this->sock->disconnectFromHost();
    return this->sock->waitForDisconnected();
}

bool Client::send(cv::Mat const &)
{

}

bool Client::sendData(const QByteArray & byteArray)
{
    byteArray.data();

}






