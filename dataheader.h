#ifndef DATAHEADER_H
#define DATAHEADER_H

#include <QByteArray>
#include <QVector>
#include <QDebug>
#include <opencv2/opencv.hpp>

class DataHeader
{
public:
    DataHeader();
    ~DataHeader();

    const char * sendCharArray();
    QByteArray & receiveByteArray();
    qint32 dataType();
    const QVector<qint32> & attr();

    qint32 encode(cv::Mat const & data);
    bool decode();

private:
    char * _sendCharArray;
    QByteArray _receiveByteArray;
    qint32 _dataType;
    QVector<qint32> _attr;
};

#endif // DATAHEADER_H
