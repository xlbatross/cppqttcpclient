#ifndef __WTCPCLIENT_H__
#define __WTCPCLIENT_H__

#include <iostream>
#include <string>
#include <winsock2.h>
#include <opencv2/opencv.hpp>
#include "dataheader.h"

class WTCPClient
{
private:
    WSADATA wsaData;
    SOCKADDR_IN servAddr;
    SOCKET cSock;
    DataHeader * dataHeader;

public:
    WTCPClient();
    ~WTCPClient();
    bool connectServer(std::string serverIp = "127.0.0.1", short serverPort = 2500);
    bool sendData(const std::string & str);
    bool sendData(const cv::Mat & img);
    bool sendByteData(const char * data, const long dataSize);

    DataHeader * receiveHeader();
    bool receiveByteData(char ** data);
};
#endif /* __WTCPCLIENT_H__ */


