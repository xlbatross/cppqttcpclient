#include "request.h"
Request::Request()
    : _headerBytes(NULL)
    , _dataBytesList(NULL)
    , _headerSize(0)
{

}

Request::~Request()
{
    this->initHeaderBytes();
    this->initDataBytesList();
}

const char *Request::headerBytes()
{
    return this->_headerBytes;
}

const char * const *Request::dataBytesList()
{
    return this->_dataBytesList;
}

const int Request::headerSize()
{
    return this->_headerSize;
}

const std::vector<int> &Request::dataLengthList()
{
    return this->_dataLengthList;
}

void Request::initHeaderBytes()
{
    if (this->_headerBytes != NULL)
    {
        delete [] this->_headerBytes;
        this->_headerBytes = NULL;
    }
}

void Request::initDataBytesList()
{
    if (this->_dataBytesList != NULL && this->_dataLengthList.size() > 0)
    {
        for (int i = 0; i < this->_dataLengthList.size(); i++)
        {
            if (this->_dataBytesList[i] != NULL)
                delete [] this->_dataBytesList[i];
        }
        delete [] this->_dataBytesList;
        this->_dataBytesList = NULL;
        this->_dataLengthList.resize(0);
    }
}


ReqImage::ReqImage(const cv::Mat &img)
    : Request()
{
    /*
     * reqImage header
     * receiveCount : 1 <32bit, 4byte, int>
     * requestType : 1(image) (32bite, 4byte, int)
     * totalDataSize : (240 * 320 * 3) <32bit, 4byte, int>
     */

    this->_headerSize = sizeof(int) * 3;
    int receiveCount = 1;
    int requestType = Request::Image;
    int totalDataSize = img.total() * img.channels();

    this->_headerBytes = new char[this->_headerSize];
    memcpy(this->_headerBytes + sizeof(int) * 0, &receiveCount, sizeof(int)); // dataCount
    memcpy(this->_headerBytes + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
    memcpy(this->_headerBytes + sizeof(int) * 2, &totalDataSize, sizeof(int)); // attrSize;

    this->_dataLengthList.resize(receiveCount);
    this->_dataBytesList = new char * [receiveCount]();
    this->_dataBytesList[0] = new char[totalDataSize];
    memcpy(this->_dataBytesList[0], (char *) img.data, totalDataSize);
    this->_dataLengthList[0] = totalDataSize;
}

ReqRoomList::ReqRoomList()
    : Request()
{
    /*
     * reqRoomList header
     * receiveCount : 0 <32bit, 4byte, int>
     * requestType : 2(RoomList) (32bite, 4byte, int)
     * totalDataSize : 0 <32bit, 4byte, int>
     */

    this->_headerSize = sizeof(int) * 3;
    int receiveCount = 0;
    int requestType = Request::RoomList;
    int totalDataSize = 0;

    this->_headerBytes = new char[this->_headerSize];
    memcpy(this->_headerBytes + sizeof(int) * 0, &receiveCount, sizeof(int)); // dataCount
    memcpy(this->_headerBytes + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
    memcpy(this->_headerBytes + sizeof(int) * 2, &totalDataSize, sizeof(int)); // attrSize;
}

ReqMakeRoom::ReqMakeRoom(std::string roomName)
    : Request()
{
    /*
     * reqMakeRoom header
     * receiveCount : 1 <32bit, 4byte, int>
     * requestType : 3(reqMakeRoom) (32bite, 4byte, int)
     * totalDataSize : roomName length <32bit, 4byte, int>
     */

    this->_headerSize = sizeof(int) * 4;
    int receiveCount = 1;
    int requestType = Request::MakeRoom;
    int totalDataSize = roomName.size();

    this->_headerBytes = new char[this->_headerSize];
    memcpy(this->_headerBytes + sizeof(int) * 0, &receiveCount, sizeof(int)); // dataCount
    memcpy(this->_headerBytes + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
    memcpy(this->_headerBytes + sizeof(int) * 2, &totalDataSize, sizeof(int)); // attrSize;

    this->_dataLengthList.resize(receiveCount);
    this->_dataBytesList = new char * [receiveCount]();
    this->_dataBytesList[0] = new char[totalDataSize];
    memcpy(this->_dataBytesList[0], roomName.c_str(), totalDataSize);
    this->_dataLengthList[0] = totalDataSize;
}

ReqEnterRoom::ReqEnterRoom(std::string ip, int port)
    : Request()
{
    /*
     * reqEnterRoom header
     * receiveCount : 2 <32bit, 4byte, int>
     * requestType : 4(reqEnterRoom) (32bite, 4byte, int)
     * totalDataSize : iplength + portlength <32bit, 4byte, int>
     */

    this->_headerSize = sizeof(int) * 5;
    int receiveCount = 2;
    int requestType = Request::EnterRoom;
    int totalDataSize = ip.size() + 4;

    this->_headerBytes = new char[this->_headerSize];
    memcpy(this->_headerBytes + sizeof(int) * 0, &receiveCount, sizeof(int)); // dataCount
    memcpy(this->_headerBytes + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
    memcpy(this->_headerBytes + sizeof(int) * 2, &totalDataSize, sizeof(int)); // attrSize;

    this->_dataLengthList.resize(receiveCount);
    this->_dataBytesList = new char * [receiveCount]();

    this->_dataBytesList[0] = new char[ip.size()];
    memcpy(this->_dataBytesList[0], ip.c_str(), ip.size());
    this->_dataLengthList[0] = ip.size();

    this->_dataBytesList[1] = new char[sizeof(int)];
    memcpy(this->_dataBytesList[1], (char *)(&port), sizeof(int));
    this->_dataLengthList[1] = sizeof(int);
}

ReqLeaveRoom::ReqLeaveRoom()
    : Request()
{
    /*
     * reqLeaveRoom header
     * receiveCount : 0 <32bit, 4byte, int>
     * requestType : 5(LeaveRoom) (32bite, 4byte, int)
     * totalDataSize : 0 <32bit, 4byte, int>
     */

    this->_headerSize = sizeof(int) * 3;
    int receiveCount = 0;
    int requestType = Request::LeaveRoom;
    int totalDataSize = 0;

    this->_headerBytes = new char[this->_headerSize];
    memcpy(this->_headerBytes + sizeof(int) * 0, &receiveCount, sizeof(int)); // dataCount
    memcpy(this->_headerBytes + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
    memcpy(this->_headerBytes + sizeof(int) * 2, &totalDataSize, sizeof(int)); // attrSize;
}
