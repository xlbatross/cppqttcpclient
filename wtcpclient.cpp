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

//bool WTCPClient::sendReqImage(const cv::Mat & img)
//{
//    int headerDataSize = this->dataHeader->encodeReqImage(img);

//    if (!this->sendByteData(this->dataHeader->sendByteArray(), headerDataSize)
//     || !this->sendByteData((char *)(img.data), img.total() * img.channels()))
//        return false;

//    return true;
//}

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

bool WTCPClient::sendRequest(Request * request)
{
    if (!this->sendByteData(request->headerBytes(), request->headerSize()))
        return false;
    for (int i = 0; i < request->dataLengthList().size(); i++)
    {
        std::cout << i << std::endl;
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
    // ??? ????????? ????????? little endian??????
    // ?????? ????????? ??? big endian?????? ???????????? ?????????.
    // ??? ????????? 16 0 0 0??? ??? ?????? ???????????? 4????????? 16?????? ????????????
    // ????????? ????????? ?????? ??? little endian?????? ???????????? ????????? 4????????? 268,435,456?????? ?????????.
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

//?????????????????? ?????? ?????????????????? ?????????
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









