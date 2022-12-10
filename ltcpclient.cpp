#include "ltcpclient.h"
LTCPClient::LTCPClient()
    : dataHeader(new DataHeader)
{
    this->cSock = socket(PF_INET, SOCK_STREAM, 0);
}

LTCPClient::~LTCPClient()
{
    delete dataHeader;
    close(this->cSock);
}

bool LTCPClient::connectServer(std::string serverIp, short serverPort)
{
    memset(&this->servAddr, 0, sizeof(this->servAddr));
    this->servAddr.sin_family = AF_INET;
    this->servAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
    this->servAddr.sin_port = htons(serverPort);

    if (connect(this->cSock, (struct sockaddr *) &this->servAddr, sizeof(this->servAddr)) == -1)
        return false;
    else
        return true;
}

bool LTCPClient::sendReqRoomList()
{
    int headerDataSize = this->dataHeader->encodeReqRoomList();
    if (!this->sendByteData(this->dataHeader->sendByteArray(), headerDataSize))
        return false;
    return true;
}

bool LTCPClient::sendReqImage(const cv::Mat & img)
{
    int headerDataSize = this->dataHeader->encodeReqImage(img);

    if (!this->sendByteData(this->dataHeader->sendByteArray(), headerDataSize)
     || !this->sendByteData((char *)(img.data), img.total() * img.channels()))
        return false;

    return true;
}

bool LTCPClient::sendByteData(const char *data, const int dataSize)
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
    std::cout << dataSize << std::endl;
    if (write(this->cSock, (char *)(&dataSize), sizeof(int)) == -1
     || write(this->cSock, data, dataSize) == -1)
        return false;
    return true;
}

DataHeader *LTCPClient::receiveHeader()
{
    this->receiveByteData(this->dataHeader->receiveByteArray());
    return this->dataHeader;
}

int LTCPClient::receiveByteData(char **data)
{
    int dataSize = -1;
    int packetSize = 0;
    int packet = 0;
    int totalReceiveSize = 0;
    bool isSizeReceive = true;

    if (read(this->cSock, (char *)(&dataSize), sizeof(int)) == -1)
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
            packet = read(this->cSock, *data + totalReceiveSize, packetSize);
            if (packet == -1)
            {
                dataSize = -1;
                break;
            }
            totalReceiveSize += packet;
        }
    }
    return dataSize;
}









