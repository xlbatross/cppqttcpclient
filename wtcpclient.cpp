#include "wtcpclient.h"

WTCPClient::WTCPClient()
    : dataHeader(new DataHeader)
{
    WSAStartup(MAKEWORD(2, 2), &this->wsaData);
    this->cSock = socket(PF_INET, SOCK_STREAM, 0);
}

WTCPClient::~WTCPClient()
{
    delete dataHeader;
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

bool WTCPClient::sendReqRoomList()
{
    long headerDataSize = this->dataHeader->encodeReqRoomList();
    if (!this->sendByteData(this->dataHeader->sendByteArray(), headerDataSize))
        return false;
    return true;
}

bool WTCPClient::sendReqImage(const cv::Mat & img)
{
    long headerDataSize = this->dataHeader->encodeReqImage(img);

    if (!this->sendByteData(this->dataHeader->sendByteArray(), headerDataSize)
     || !this->sendByteData((char *)(img.data), img.total() * img.channels()))
        return false;

    return true;
}

bool WTCPClient::sendByteData(const char *data, const long dataSize)
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
    if (send(this->cSock, (char *)(&dataSize), sizeof(long), 0) == SOCKET_ERROR
     || send(this->cSock, data, dataSize, 0) == SOCKET_ERROR)
        return false;
    return true;
}

DataHeader *WTCPClient::receiveHeader()
{
    this->receiveByteData(this->dataHeader->receiveByteArray());
    return this->dataHeader;
}

long WTCPClient::receiveByteData(char **data)
{
    long dataSize = -1;
    long packetSize = 0;
    long packet = 0;
    long totalReceiveSize = 0;
    bool isSizeReceive = true;

    if (recv(this->cSock, (char *)(&dataSize), sizeof(long), 0) == SOCKET_ERROR)
    {
        isSizeReceive = false;
    }

    if (isSizeReceive)
    {
        if (*data != NULL)
            delete [] *data;

        *data = new char[dataSize];

        while (totalReceiveSize < dataSize)
        {
            packetSize = (totalReceiveSize + 1024 < dataSize) ? 1024 : dataSize - totalReceiveSize;
            packet = recv(this->cSock, *data + totalReceiveSize, packetSize, 0);
            if (packet == SOCKET_ERROR)
            {
                dataSize = -1;
                break;
            }
            totalReceiveSize += packet;
        }
    }
    return dataSize;
}









