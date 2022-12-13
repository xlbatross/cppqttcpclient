#include "wtcpclient.h"
WTCPClient::WTCPClient()
{
    WSAStartup(MAKEWORD(2, 2), &this->wsaData);
    this->cSock = socket(PF_INET, SOCK_STREAM, 0);
}

WTCPClient::~WTCPClient()
{
    closesocket(this->cSock);
    WSACleanup();
}

bool WTCPClient::connectServer(std::string serverIp, short serverPort)
{
    memset(&this->servAddr, 0, sizeof(this->servAddr));
    this->servAddr.sin_family = AF_INET;
    this->servAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
    this->servAddr.sin_port = htons(serverPort);

    if (connect(this->cSock, (SOCKADDR*) &this->servAddr, sizeof(this->servAddr)) == SOCKET_ERROR)
        return false;
    else
        return true;
}

bool WTCPClient::sendReqImage(const cv::Mat & img)
{
    ReqImage reqImage(img);
    return this->sendRequest(&reqImage);
}

bool WTCPClient::sendReqRoomList()
{
    ReqRoomList reqRoomList;
    return this->sendRequest(&reqRoomList);
}

bool WTCPClient::sendReqMakeRoom(const std::string &roomName)
{
    ReqMakeRoom reqMakeRoom(roomName);
    return this->sendRequest(&reqMakeRoom);
}

bool WTCPClient::sendReqEnterRoom(const std::string &ip, const int port)
{
    ReqEnterRoom reqEnterRoom(ip, port);
    return this->sendRequest(&reqEnterRoom);
}

bool WTCPClient::sendReqLeaveRoom()
{
    ReqLeaveRoom reqLeaveRoom;
    return this->sendRequest(&reqLeaveRoom);
}

bool WTCPClient::sendRequest(Request * request)
{
    if (!this->sendByteData(request->headerBytes(), request->headerSize()))
        return false;
    for (int i = 0; i < request->dataLengthList().size(); i++)
    {
        if (!this->sendByteData(request->dataBytesList()[i], request->dataLengthList()[i]))
        {
            return false;
        }
    }
    return true;
}

bool WTCPClient::sendByteData(const char *data, const int dataSize)
{
//    for (int i = 0; i < 4; i++)
//    {
//        std::cout << (int)(*((char *)(&dataSize) + i)) << " ";
//    }
//    std::cout << std::endl;
    // 이 컴퓨터 자체가 little endian이다
    // 다만 송신할 때 big endian으로 송신되지 않는다.
    // 즉 데이터 16 0 0 0이 이 코드 상에서는 4바이트 16으로 읽히지만
    // 파이썬 쪽에서 받을 때 little endian으로 판단하지 않으면 4바이트 268,435,456으로 보인다.
    if (send(this->cSock, (char *)(&dataSize), sizeof(int), 0) == SOCKET_ERROR
     || send(this->cSock, data, dataSize, 0) == SOCKET_ERROR)
        return false;
    return true;
}

//DataHeader *WTCPClient::receive(char *** data)
//{
//    // data initilize
//    if (*data != NULL && this->dataHeader->dataCount() != 0)
//    {
//        for (int i = 0; i < this->dataHeader->dataCount(); i++)
//        {
//            if ((*data)[i] != NULL)
//                delete [] (*data)[i];
//        }
//        delete [] (*data);
//    }

//    // receive header data and decode
//    if (this->receiveByteData(this->dataHeader->receiveByteArray()) == -1
//    || !this->dataHeader->decode())
//        return NULL;

//    // receive real data
//    *data = new char * [this->dataHeader->dataCount()]();
//    for (int i = 0; i < this->dataHeader->dataCount(); i++)
//    {
//        if (this->receiveByteData(&((*data)[i])) == -1)
//            return NULL;
//    }

//    return this->dataHeader;
//}

//헤더데이터를 받고 실제데이터를 받는다
int WTCPClient::receive(char **headerBytes, char ***dataBytesList, std::vector<int> & dataLengthList)
{
    int dataCount = 0;
    int headSize = this->receiveByteData(headerBytes);
    // receive header data
    if (headSize == -1)
        return -1;


    memcpy(&dataCount, *headerBytes, sizeof(int));
    dataLengthList.resize(dataCount);

    // receive real data
    *dataBytesList = new char * [dataCount]();
    for (int i = 0; i < dataCount; i++)
    {
        dataLengthList[i] = this->receiveByteData(&((*dataBytesList)[i]));
        if (dataLengthList[i] == -1)
            return -1;
    }

    return headSize;
}


int WTCPClient::receiveByteData(char **data)
{
    int dataSize = -1;
    int packetSize = 0;
    int packet = 0;
    int totalReceiveSize = 0;
    bool isSizeReceive = true;
    if (recv(this->cSock, (char *)(&dataSize), sizeof(int), 0) == SOCKET_ERROR)
        return -1;

    if (*data != NULL)
        delete [] *data;

    *data = new char[dataSize];

    while (totalReceiveSize < dataSize)
    {
        packetSize = (totalReceiveSize + 1024 < dataSize) ? 1024 : dataSize - totalReceiveSize;
        packet = recv(this->cSock, (*data) + totalReceiveSize, packetSize, 0);
        if (packet == SOCKET_ERROR)
            return -1;
        totalReceiveSize += packet;
    }
    return dataSize;
}









