#ifndef DATAHEADER_H
#define DATAHEADER_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class DataHeader
{
public:
    enum RequestType{reqImage = 1, reqRoomList};
    enum ResponseType{resImage = 1, resRoomList};

    DataHeader();
    ~DataHeader();

    const char * sendByteArray();
    char ** receiveByteArray();
    long responseType();
    long dataCount();
    const std::vector<long> & attr();

    long encodeReqRoomList();
    long encodeReqImage(const cv::Mat & data);

    bool decode();

private:
    char * _sendByteArray;
    char * _receiveByteArray;
    long _responseType;
    long _dataCount;
    std::vector<long> _attr;
};

#endif // DATAHEADER_H
