#ifndef DATAHEADER_COPY_H
#define DATAHEADER_COPY_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class DataHeader
{
public:
    enum RequestType{reqImage = 1, reqRoomList, reqMakeRoom, reqEnterRoom};
    enum ResponseType{resImage = 1, resRoomList, resMakeRoom, resEnterRoom};

    DataHeader();
    ~DataHeader();

    const char * sendByteArray();
    char ** receiveByteArray();
    int dataCount();
    int responseType();
    int attrSize();
    const std::vector<int> & attr();

    int encodeReqImage(const cv::Mat & data);
    int encodeReqRoomList();
    int encodeReqMakeRoom(std::string roomName);
    int encodeReqEnterRoom(std::string ip, int port);

    bool decode();

private:
    char * _sendByteArray;
    char * _receiveByteArray;
    int _dataCount;
    int _responseType;
    int _attrSize;
    std::vector<int> _attr;
};

#endif // DATAHEADER_COPY_H
