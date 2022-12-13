#ifndef __WTCPCLIENT_H__
#define __WTCPCLIENT_H__

#include <iostream>
#include <string>
#include <winsock2.h>
#include <opencv2/opencv.hpp>
#include "request.h"

class WTCPClient
{
private:
    WSADATA wsaData;
    SOCKADDR_IN servAddr;
    SOCKET cSock;
    DWORD timeout;

public:
    WTCPClient();
    ~WTCPClient();
    bool connectServer(std::string serverIp = "127.0.0.1", short serverPort = 2500);

    bool sendReqImage(const cv::Mat & img);
    bool sendReqRoomList();
    bool sendReqMakeRoom(const std::string & roomName);
    bool sendReqEnterRoom(const std::string & ip, const int port);
    bool sendReqLeaveRoom();
<<<<<<< HEAD
    bool sendReqLogin(const std::string & num, const std::string &pw );
    bool sendReqSignUp(const std::string & name, const std::string & num, const std::string & pw, const std::string & cate);
=======
>>>>>>> eb5fbce6a906683b9992b73f22f876f3e0feff9b
    
    bool sendRequest(Request * request);
    bool sendByteData(const char * data, const int dataSize);//실질적으로 데이터를 보내는 것

    int receive(char ** headerBytes, char *** dataBytesList, std::vector<int> & dataLengthList);
    int receiveByteData(char ** data);//실질적으로 데이터를 받는 것
};
#endif /* __WTCPCLIENT_H__ */


