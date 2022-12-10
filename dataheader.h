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
    int responseType();
    int dataCount();
    const std::vector<int> & attr();

    int encodeReqRoomList();
    int encodeReqImage(const cv::Mat & data);

    bool decode();

private:
    char * _sendByteArray;
    char * _receiveByteArray;
    int _responseType;
    int _dataCount;
    std::vector<int> _attr;
};

#endif // DATAHEADER_H
