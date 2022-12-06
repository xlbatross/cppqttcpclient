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

bool Client::send(cv::Mat const & data)
{
    QJsonDocument doc;
    QJsonObject body;
    QJsonObject attr;
    body["dataType"] = "Image";
    attr["width"] = data.cols;
    attr["height"] = data.rows;
    attr["bpp"] = data.channels();
    body["attribute"] = attr;
    doc.setObject(body);
    return sendData(doc.toJson());
}

bool Client::sendData(const QByteArray & data)
{
    this->sock->write(data);
    return this->sock->waitForBytesWritten();
}






