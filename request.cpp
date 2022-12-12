#include "request.h"

//DataHeader::DataHeader()
//{
//    this->_dataCount = 0;
//    this->_sendByteArray = NULL;
//    this->_receiveByteArray = NULL;
//}

//DataHeader::~DataHeader()
//{
//    if (this->_sendByteArray != NULL)
//        delete [] this->_sendByteArray;
//    if (this->_receiveByteArray != NULL)
//        delete [] this->_receiveByteArray;
//}

//const char *DataHeader::sendByteArray()
//{
//    return this->_sendByteArray;
//}

//char ** DataHeader::receiveByteArray()
//{
//    return &this->_receiveByteArray;
//}

//int DataHeader::responseType()
//{
//    return this->_responseType;
//}

//int DataHeader::attrSize()
//{
//    return this->_attrSize;
//}

//int DataHeader::dataCount()
//{
//    return this->_dataCount;
//}

//const std::vector<int> &DataHeader::attr()
//{
//    return this->_attr;
//}

//int DataHeader::encodeReqImage(const cv::Mat &data)
//{
//    /*
//     * reqImage header
//     * datacount : 1 <32bit, 4byte, int>
//     * requestType : 1(reqimage) (32bite, 4byte, int)
//     * element attr size : 3
//     * attr:
//     *     height <32bit, 4byte, int>
//     *     width <32bit, 4byte, int>
//     *     channels <32bit, 4byte, int>
//     */

//    try
//    {
//        int dataCount = 1;
//        int requestType = reqImage; // image
//        int attrSize = 3;
//        int height = data.rows;
//        int width = data.cols;
//        int channels = data.channels();

//        if (this->_sendByteArray != NULL)
//            delete [] this->_sendByteArray;
//        this->_sendByteArray = new char[sizeof(int) * 6];
//        memcpy(this->_sendByteArray + sizeof(int) * 0, &dataCount, sizeof(int)); // dataCount
//        memcpy(this->_sendByteArray + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
//        memcpy(this->_sendByteArray + sizeof(int) * 2, &attrSize, sizeof(int)); // attrSize;
//        memcpy(this->_sendByteArray + sizeof(int) * 3, &height, sizeof(int)); // height
//        memcpy(this->_sendByteArray + sizeof(int) * 4, &width, sizeof(int)); // width
//        memcpy(this->_sendByteArray + sizeof(int) * 5, &channels, sizeof(int)); // channels
//        return sizeof(int) * 6;
//    }
//    catch (const std::bad_alloc &)
//    {
//        return -1;
//    }
//}

//int DataHeader::encodeReqRoomList()
//{
//    /*
//     * reqRoomList header
//     * datacount : 0 <32bit, 4byte, int>
//     * requestType : 2(reqRoomList) (32bite, 4byte, int)
//     * attr by one element size : 0 <32bit, 4byte, int>
//     * attr:
//     *     none
//     */

//    try
//    {
//        int dataCount = 0;
//        int requestType = reqRoomList;
//        int attrSize = 0;

//        if (this->_sendByteArray != NULL)
//            delete [] this->_sendByteArray;
//        this->_sendByteArray = new char[sizeof(int) * 3];
//        memcpy(this->_sendByteArray + sizeof(int) * 0, &dataCount, sizeof(int)); // dataCount
//        memcpy(this->_sendByteArray + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
//        memcpy(this->_sendByteArray + sizeof(int) * 2, &attrSize, sizeof(int)); // attrSize;
//        return sizeof(int) * 3;
//    }
//    catch (const std::bad_alloc &)
//    {
//        return -1;
//    }
//}

//int DataHeader::encodeReqMakeRoom(std::string roomName)
//{
//    /*
//     * reqMakeRoom header
//     * datacount : 1 <32bit, 4byte, int>
//     * requestType : 3(reqMakeRoom) (32bite, 4byte, int)
//     * attr by one element size : 1 <32bit, 4byte, int>
//     * attr:
//     *    roomName length <32bit, 4byte, int>
//     */

//    try
//    {
//        int dataCount = 1;
//        int requestType = reqMakeRoom;
//        int attrSize = 1;
//        int roomNameLength = roomName.size();

//        if (this->_sendByteArray != NULL)
//            delete [] this->_sendByteArray;
//        this->_sendByteArray = new char[sizeof(int) * 4];
//        memcpy(this->_sendByteArray + sizeof(int) * 0, &dataCount, sizeof(int)); // dataCount
//        memcpy(this->_sendByteArray + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
//        memcpy(this->_sendByteArray + sizeof(int) * 2, &attrSize, sizeof(int)); // attrSize;
//        memcpy(this->_sendByteArray + sizeof(int) * 3, &roomNameLength, sizeof(int));
//        return sizeof(int) * 4;
//    }
//    catch (const std::bad_alloc &)
//    {
//        return -1;
//    }
//}

//int DataHeader::encodeReqEnterRoom(std::string ip, int port)
//{
//    /*
//     * reqEnterRoom header
//     * datacount : 1 <32bit, 4byte, int>
//     * requestType : 4(reqEnterRoom) (32bite, 4byte, int)
//     * attr by one element size : 2 <32bit, 4byte, int>
//     * attr:
//     *    ip length <32bit, 4byte, int>
//     *    port length <32bit, 4byte, int>
//     */

//    try
//    {
//        int dataCount = 1;
//        int requestType = reqEnterRoom;
//        int attrSize = 2;
//        int ipLength = ip.size();
//        int portLength = sizeof(int);

//        if (this->_sendByteArray != NULL)
//            delete [] this->_sendByteArray;
//        this->_sendByteArray = new char[sizeof(int) * 5];
//        memcpy(this->_sendByteArray + sizeof(int) * 0, &dataCount, sizeof(int)); // dataCount
//        memcpy(this->_sendByteArray + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
//        memcpy(this->_sendByteArray + sizeof(int) * 2, &attrSize, sizeof(int)); // attrSize;
//        memcpy(this->_sendByteArray + sizeof(int) * 3, &ipLength, sizeof(int));
//        memcpy(this->_sendByteArray + sizeof(int) * 4, &portLength, sizeof(int));
//        return sizeof(int) * 5;
//    }
//    catch (const std::bad_alloc &)
//    {
//        return -1;
//    }
//}

//bool DataHeader::decode()
//{
//    /*
//     * resImage header example
//     * datacount : 1 <32bit, 4byte, int>
//     * responseType : 1(resImage) (32bite, 4byte, int)
//     * attr by one element size :  3
//     * attr:
//     *     height <32bit, 4byte, int>
//     *     width <32bit, 4byte, int>
//     *     channels <32bit, 4byte, int>
//     */

//    this->_attr.clear();
//    memcpy(&(this->_dataCount), this->_receiveByteArray + sizeof(int) * 0, sizeof(int));
//    memcpy(&(this->_responseType), this->_receiveByteArray + sizeof(int) * 1, sizeof(int));
//    memcpy(&(this->_attrSize), this->_receiveByteArray + sizeof(int) * 2, sizeof(int));

//    this->_attr.resize(this->_dataCount * this->_attrSize);

//    switch(this->_responseType)
//    {
//    case resImage :
//    case resRoomList:
//    case resMakeRoom:
//        for (int i = 0; i < this->_attr.size(); i++)
//            memcpy(&(this->_attr[i]), this->_receiveByteArray + sizeof(int) * (3 + i), sizeof(int));
//        return true;
//        break;
//    }

//    return false;
//}


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

/////
ReqRoomList::ReqRoomList()
    : Request()
{
    /*
     * reqRoomList header
     * receiveCount : 0 <32bit, 4byte, int>
     * requestType : 2(RoomList) (32bite, 4byte, int)
     * dataSize : 0 <32bit, 4byte, int>
     * dataType:
     *     none
     */

    this->_headerSize = sizeof(int) * 3;
    int receiveCount = 0;
    int requestType = Request::RoomList;
    int dataSize = 0;

    this->_headerBytes = new char[this->_headerSize];
    memcpy(this->_headerBytes + sizeof(int) * 0, &receiveCount, sizeof(int)); // dataCount
    memcpy(this->_headerBytes + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
    memcpy(this->_headerBytes + sizeof(int) * 2, &dataSize, sizeof(int)); // attrSize;
}

ReqMakeRoom::ReqMakeRoom(std::string roomName)
    : Request()
{
    /*
     * reqMakeRoom header
     * receiveCount : 1 <32bit, 4byte, int>
     * requestType : 3(reqMakeRoom) (32bite, 4byte, int)
     * dataSize : 1 <32bit, 4byte, int>
     * dataType :
     *      0(string) <32bit, 4byte, int>
     */

    this->_headerSize = sizeof(int) * 4;
    int receiveCount = 1;
    int requestType = Request::MakeRoom;
    int dataSize = 1;
    int firstDataType = Request::String;

    this->_headerBytes = new char[this->_headerSize];
    memcpy(this->_headerBytes + sizeof(int) * 0, &receiveCount, sizeof(int)); // dataCount
    memcpy(this->_headerBytes + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
    memcpy(this->_headerBytes + sizeof(int) * 2, &dataSize, sizeof(int)); // attrSize;
    memcpy(this->_headerBytes + sizeof(int) * 3, &firstDataType, sizeof(int));

    this->_dataLengthList.resize(1);
    this->_dataBytesList = new char * [1]();
    this->_dataBytesList[0] = new char[roomName.size()];
    memcpy(this->_dataBytesList[0], roomName.c_str(), roomName.size());
    this->_dataLengthList[0] = roomName.size();
}

ReqEnterRoom::ReqEnterRoom(std::string ip, int port)
    : Request()
{
    /*
     * reqEnterRoom header
     * receiveCount : 2 <32bit, 4byte, int>
     * requestType : 4(reqEnterRoom) (32bite, 4byte, int)
     * dataSize : 2 <32bit, 4byte, int>
     * dataType:
     *    0 : string <32bit, 4byte, int>
     *    1 : int <32bit, 4byte, int>
     */

    this->_headerSize = sizeof(int) * 5;
    int receiveCount = 2;
    int requestType = Request::EnterRoom;
    int dataSize = 2;
    int firstDataType = Request::String;
    int secondDataType = Request::Int;

    this->_headerBytes = new char[this->_headerSize];
    memcpy(this->_headerBytes + sizeof(int) * 0, &receiveCount, sizeof(int)); // dataCount
    memcpy(this->_headerBytes + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
    memcpy(this->_headerBytes + sizeof(int) * 2, &dataSize, sizeof(int)); // attrSize;
    memcpy(this->_headerBytes + sizeof(int) * 3, &firstDataType, sizeof(int));
    memcpy(this->_headerBytes + sizeof(int) * 4, &secondDataType, sizeof(int));

    this->_dataLengthList.resize(receiveCount);
    this->_dataBytesList = new char * [receiveCount]();

    this->_dataBytesList[0] = new char[ip.size()];
    memcpy(this->_dataBytesList[0], ip.c_str(), ip.size());
    this->_dataLengthList[0] = ip.size();

    this->_dataBytesList[1] = new char[sizeof(int)];
    memcpy(this->_dataBytesList[1], (char *)(&port), sizeof(int));
    this->_dataLengthList[1] = sizeof(int);
}
