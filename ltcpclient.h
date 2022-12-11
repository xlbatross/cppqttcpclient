#ifndef LTCPCLIENT_H
#define LTCPCLIENT_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <opencv2/opencv.hpp>
#include "request.h"

class LTCPClient
{
private:
    struct sockaddr_in servAddr;
    int cSock;

public:
    LTCPClient();
    ~LTCPClient();
    bool connectServer(std::string serverIp = "127.0.0.1", short serverPort = 2500);

//    bool sendReqImage(const cv::Mat & img);
    bool sendReqRoomList();
    bool sendReqMakeRoom(const std::string & roomName);

    bool sendRequest(Request * request);
    bool sendByteData(const char * data, const int dataSize);

    int receive(char ** headerBytes, char *** dataBytesList, std::vector<int> & dataLengthList);
    int receiveByteData(char ** data);
};
#endif /* LTCPCLIENT_H */


