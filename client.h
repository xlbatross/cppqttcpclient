#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>
#include <opencv2/opencv.hpp>
#include <QJsonDocument>
#include <QJsonObject>
#include "dataheader.h"

class Client
{
public:
    Client(DataHeader * dataHeader);
    ~Client();

    bool connectToHost(QString serverIp = "127.0.0.1", quint16 portNum = 2500);
    bool disconnectToHost();

    bool send(cv::Mat const & data);
    bool sendData(const char * data, const qint32 dataSize);;
    QByteArray dataSizeToByteArray(const qint32 dataSize);

    bool receiveData(QByteArray & receiver);
    qint32 byteArrayToDataSize(QByteArray dataSize);

private:
    QTcpSocket * sock;
    DataHeader * dataHeader;
};

#endif // CLIENT_H
