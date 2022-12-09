#ifndef DATAHEADER_H
#define DATAHEADER_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class DataHeader
{
public:
    enum Type{Image = 1};

    DataHeader();
    ~DataHeader();

    const char * sendByteArray();
    char ** receiveByteArray();
    long dataType();
    const std::vector<long> & attr();

    long encode(cv::Mat const & data);
    bool decode();

private:
    char * _sendByteArray;
    char * _receiveByteArray;
    long _dataType;
    std::vector<long> _attr;
};

#endif // DATAHEADER_H
