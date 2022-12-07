#include "client.h"

Client::Client(QObject * parent)
    : sock(new QTcpSocket((QObject *) parent))
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
    /* JSON template
     * {
     *     "dataType" : "Image",
     *     "attribute" :
     *     {
     *          "width" : <int>,
     *          "height" : <int>,
     *          "channels" : <int>
     *     },
     *     "data" : <cv::Mat data to base64 encoding string>
     * }
     */
    QJsonDocument doc;
    QJsonObject body;
    QJsonObject attr;

    body["dataType"] = "Image";
    attr["width"] = data.cols;
    attr["height"] = data.rows;
    attr["channels"] = data.channels();
    body["attribute"] = attr;
    body["data"] = QString::fromLatin1(QByteArray((char *)data.data, data.cols * data.rows * data.channels()).toBase64());
    doc.setObject(body);

    return this->sendData(doc.toJson());
}

bool Client::sendData(const QByteArray & data)
{
    this->sock->write(data);
    return this->sock->waitForBytesWritten(-1);
}






