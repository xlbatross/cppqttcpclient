#include "ltcpclient.h"
LTCPClient::LTCPClient()
{
    this->cSock = socket(PF_INET, SOCK_STREAM, 0);
    
    // 소켓 수신 타임 아웃 세팅
    struct timeval tv;
    tv.tv_sec = 5;
    setsockopt(this->cSock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

}

LTCPClient::~LTCPClient()
{
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

bool LTCPClient::sendReqImage(const cv::Mat & img)
{
    ReqImage reqImage(img);
    return this->sendRequest(&reqImage);
}

bool LTCPClient::sendReqRoomList()
{
    ReqRoomList reqRoomList;
    return this->sendRequest(&reqRoomList);
}

bool LTCPClient::sendReqMakeRoom(const std::string &roomName)
{
    ReqMakeRoom reqMakeRoom(roomName);
    return this->sendRequest(&reqMakeRoom);
}

bool LTCPClient::sendReqEnterRoom(const std::string &ip, const int port)
{
    ReqEnterRoom reqEnterRoom(ip, port);
    return this->sendRequest(&reqEnterRoom);
}

bool LTCPClient::sendReqLeaveRoom()
{
    ReqLeaveRoom reqLeaveRoom;
    return this->sendRequest(&reqLeaveRoom);
}

bool LTCPClient::sendReqLogin(const std::string &num, const std::string &pw)
{
    ReqLogin reqLogin(num, pw);
    return this->sendRequest(&reqLogin);
}

bool LTCPClient::sendReqSignUp(const std::string &name, const std::string &num, const std::string &pw, const std::string &cate)
{
    ReqSignUp reqSignUp(name,num,pw,cate);
    return this->sendRequest(&reqSignUp);
}

bool LTCPClient::sendReqChat(const std::string & text)
{
    ReqChat reqChat(text);
    return this->sendRequest(&reqChat);
}

bool LTCPClient::sendRequest(Request * request)
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
    if (write(this->cSock, (char *)(&dataSize), sizeof(int)) == -1
     || write(this->cSock, data, dataSize) == -1)
        return false;
    return true;
}

int LTCPClient::receive(char **headerBytes, char ***dataBytesList, std::vector<int> & dataLengthList)
{
    int dataCount = 0;
    int totalDataSize = 0;
    int receiveTotalSize = 0;
    int headSize = this->receiveByteData(headerBytes);
    // receive header data
    if (headSize < 0)
        return headSize;

    memcpy(&dataCount, *headerBytes, sizeof(int));
    memcpy(&totalDataSize, *headerBytes + sizeof(int) * 2, sizeof(int));
    
    dataLengthList.resize(dataCount);

    // receive real data
    *dataBytesList = new char * [dataCount]();
    for (int i = 0; i < dataCount; i++)
    {
        dataLengthList[i] = this->receiveByteData(&((*dataBytesList)[i]));
        if (dataLengthList[i] < 0)
            return dataLengthList[i];
        else
            receiveTotalSize += dataLengthList[i];
    }

    if (receiveTotalSize != totalDataSize)
        return -2;
    else
        return headSize;
}

int LTCPClient::receiveByteData(char **data)
{
    int dataSize = -1;
    int packetSize = 0;
    int packet = 0;
    int totalReceiveSize = 0;
    bool isSizeReceive = true;

    int error = read(this->cSock, (char *)(&dataSize), sizeof(int));

    if (error == -1)
    {
        if (errno == EAGAIN)
            return -2;
        else
            return -1;
    }
        

    if (*data != NULL)
        delete [] *data;

    *data = new char[dataSize];

    while (totalReceiveSize < dataSize)
    {
        packetSize = (totalReceiveSize + 1024 < dataSize) ? 1024 : dataSize - totalReceiveSize;
        packet = read(this->cSock, *data + totalReceiveSize, packetSize);
        if (packet == -1)
        {
            if (errno == EAGAIN)
                return -2;
            else
                return -1;
        }
            
        totalReceiveSize += packet;
    }
    return dataSize;
}









