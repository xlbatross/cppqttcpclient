#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>
#include <opencv2/opencv.hpp>
#include <QJsonDocument>
#include <QJsonObject>

class Client
{
public:
    Client(QObject * parent);
    ~Client();

    bool connectToHost(QString serverIp = "127.0.0.1", quint16 portNum = 2500);
    bool disconnectToHost();

    bool send(cv::Mat const & data);
    bool sendData(const char * data, const qint32 dataSize);;
    QByteArray dataSizeToByteArray(const qint32 dataSize);

    QJsonDocument receive();
    QByteArray receiveData();
    qint32 byteArrayToDataSize(QByteArray dataSize);

private:
    QTcpSocket * sock;
    char * header;
};

#endif // CLIENT_H
