#include "client.h"

Client::Client(QObject * parent)
    : sock(new QTcpSocket((QObject *) parent))
{

}

Client::~Client()
{
    delete [] this->header;
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
    /*
     * header
     * datatype : 1(image) (32bite, 4byte, int)
     * attr:
     *     height <32bit, 4byte, int>
     *     width <32bit, 4byte, int>
     *     channels <32bit, 4byte, int>
     */

    qint32 dataType = 1; // image
    qint32 height = data.rows;
    qint32 width = data.cols;
    qint32 channels = data.channels();

    bool isSend = false;

    this->header = new char[sizeof(qint32) * 4]();
    memcpy(this->header + sizeof(qint32) * 0, &dataType, sizeof(qint32)); // datatype
    memcpy(this->header + sizeof(qint32) * 1, &height, sizeof(qint32)); // height
    memcpy(this->header + sizeof(qint32) * 2, &width, sizeof(qint32)); // width
    memcpy(this->header + sizeof(qint32) * 3, &channels, sizeof(qint32)); // channels
//    for (int i = 0; i < 32; i++)
//    {
//        qDebug() << (int) this->header[i];
//    }
    isSend = this->sendData(this->header, sizeof(qint32) * 4);
    delete [] this->header;

    if (isSend)
        return this->sendData((char *)(data.data), height * width * channels);
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

QJsonDocument Client::receive()
{
    return QJsonDocument::fromJson(receiveData());
}

QByteArray Client::receiveData()
{
    QByteArray totalData;
    qint32 packetSize = 0;
    qint32 dataSize = this->byteArrayToDataSize(this->sock->read(4));

    while (totalData.size() < dataSize)
    {
        packetSize = (totalData.size() + 1024 < dataSize) ? 1024 : dataSize - totalData.size();
        totalData += this->sock->read(packetSize);
    }
    return totalData;
}

qint32 Client::byteArrayToDataSize(QByteArray dataSize)
{
    qint32 temp;
    QDataStream ds(&dataSize, QIODevice::ReadWrite);
    ds >> temp;
    return temp;
}










