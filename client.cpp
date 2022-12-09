#include "client.h"

Client::Client(DataHeader * dataHeader)
    : sock(new QTcpSocket)
    , dataHeader(dataHeader)
{

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

bool Client::send(cv::Mat const & data)
{
    qint32 headerSize = this->dataHeader->encode(data);
    bool isSend = this->sendData(this->dataHeader->sendByteArray(), headerSize);

    if (isSend)
        return this->sendData((char *)(data.data), data.total() * data.channels());
    else
        return false;
}

bool Client::sendData(const char *data, const qint32 dataSize)
{
    this->sock->write(this->dataSizeToByteArray(dataSize), sizeof(qint32));
    this->sock->write(data, dataSize);
    return this->sock->waitForBytesWritten(-1);
}

// Use qint32 to ensure that the number have 4 bytes
QByteArray Client::dataSizeToByteArray(const qint32 dataSize)
{
    // Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream ds(&temp, QIODevice::ReadWrite);
    ds << dataSize;
    return temp;
}

bool Client::receiveData(QByteArray & receiver)
{
    qint32 packetSize;
    qint32 dataSize;

    if (!this->sock->waitForReadyRead(-1))
        return false;

    dataSize = this->byteArrayToDataSize(this->sock->read(4));
    while (receiver.size() < dataSize)
    {
        packetSize = (receiver.size() + 1024 < dataSize) ? 1024 : dataSize - receiver.size();
        receiver += this->sock->read(packetSize);
    }
    return true;
}

qint32 Client::byteArrayToDataSize(QByteArray dataSize)
{
    qint32 temp;
    QDataStream ds(&dataSize, QIODevice::ReadWrite);
    ds >> temp;
    return temp;
}










