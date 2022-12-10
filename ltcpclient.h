#ifndef LTCPCLIENT_H
#define LTCPCLIENT_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <opencv2/opencv.hpp>
#include "dataheader.h"

class LTCPClient
{
private:
    struct sockaddr_in servAddr;
    int cSock;
    DataHeader * dataHeader;

public:
    LTCPClient();
    ~LTCPClient();
    bool connectServer(std::string serverIp = "127.0.0.1", short serverPort = 2500);
    bool sendData(const std::string & str);

    bool sendReqRoomList();
    bool sendReqImage(const cv::Mat & img);

    bool sendByteData(const char * data, const int dataSize);

    DataHeader * receiveHeader();
    int receiveByteData(char ** data);
};
#endif /* LTCPCLIENT_H */


