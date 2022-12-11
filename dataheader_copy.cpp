#include "dataheader.h"

DataHeader::DataHeader()
{
    this->_dataCount = 0;
    this->_sendByteArray = NULL;
    this->_receiveByteArray = NULL;
}

DataHeader::~DataHeader()
{
    if (this->_sendByteArray != NULL)
        delete [] this->_sendByteArray;
    if (this->_receiveByteArray != NULL)
        delete [] this->_receiveByteArray;
}

const char *DataHeader::sendByteArray()
{
    return this->_sendByteArray;
}

char ** DataHeader::receiveByteArray()
{
    return &this->_receiveByteArray;
}

int DataHeader::responseType()
{
    return this->_responseType;
}

int DataHeader::attrSize()
{
    return this->_attrSize;
}

int DataHeader::dataCount()
{
    return this->_dataCount;
}

const std::vector<int> &DataHeader::attr()
{
    return this->_attr;
}

int DataHeader::encodeReqImage(const cv::Mat &data)
{
    /*
     * reqImage header
     * datacount : 1 <32bit, 4byte, int>
     * requestType : 1(reqimage) (32bite, 4byte, int)
     * element attr size : 3
     * attr:
     *     height <32bit, 4byte, int>
     *     width <32bit, 4byte, int>
     *     channels <32bit, 4byte, int>
     */

    try
    {
        int dataCount = 1;
        int requestType = reqImage; // image
        int attrSize = 3;
        int height = data.rows;
        int width = data.cols;
        int channels = data.channels();

        if (this->_sendByteArray != NULL)
            delete [] this->_sendByteArray;
        this->_sendByteArray = new char[sizeof(int) * 6];
        memcpy(this->_sendByteArray + sizeof(int) * 0, &dataCount, sizeof(int)); // dataCount
        memcpy(this->_sendByteArray + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
        memcpy(this->_sendByteArray + sizeof(int) * 2, &attrSize, sizeof(int)); // attrSize;
        memcpy(this->_sendByteArray + sizeof(int) * 3, &height, sizeof(int)); // height
        memcpy(this->_sendByteArray + sizeof(int) * 4, &width, sizeof(int)); // width
        memcpy(this->_sendByteArray + sizeof(int) * 5, &channels, sizeof(int)); // channels
        return sizeof(int) * 6;
    }
    catch (const std::bad_alloc &)
    {
        return -1;
    }
}

int DataHeader::encodeReqRoomList()
{
    /*
     * reqRoomList header
     * datacount : 0 <32bit, 4byte, int>
     * requestType : 2(reqRoomList) (32bite, 4byte, int)
     * attr by one element size : 0 <32bit, 4byte, int>
     * attr:
     *     none
     */

    try
    {
        int dataCount = 0;
        int requestType = reqRoomList;
        int attrSize = 0;

        if (this->_sendByteArray != NULL)
            delete [] this->_sendByteArray;
        this->_sendByteArray = new char[sizeof(int) * 3];
        memcpy(this->_sendByteArray + sizeof(int) * 0, &dataCount, sizeof(int)); // dataCount
        memcpy(this->_sendByteArray + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
        memcpy(this->_sendByteArray + sizeof(int) * 2, &attrSize, sizeof(int)); // attrSize;
        return sizeof(int) * 3;
    }
    catch (const std::bad_alloc &)
    {
        return -1;
    }
}

int DataHeader::encodeReqMakeRoom(std::string roomName)
{
    /*
     * reqMakeRoom header
     * datacount : 1 <32bit, 4byte, int>
     * requestType : 3(reqMakeRoom) (32bite, 4byte, int)
     * attr by one element size : 1 <32bit, 4byte, int>
     * attr:
     *    roomName length <32bit, 4byte, int>
     */

    try
    {
        int dataCount = 1;
        int requestType = reqMakeRoom;
        int attrSize = 1;
        int roomNameLength = roomName.size();

        if (this->_sendByteArray != NULL)
            delete [] this->_sendByteArray;
        this->_sendByteArray = new char[sizeof(int) * 4];
        memcpy(this->_sendByteArray + sizeof(int) * 0, &dataCount, sizeof(int)); // dataCount
        memcpy(this->_sendByteArray + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
        memcpy(this->_sendByteArray + sizeof(int) * 2, &attrSize, sizeof(int)); // attrSize;
        memcpy(this->_sendByteArray + sizeof(int) * 3, &roomNameLength, sizeof(int));
        return sizeof(int) * 4;
    }
    catch (const std::bad_alloc &)
    {
        return -1;
    }
}

int DataHeader::encodeReqEnterRoom(std::string ip, int port)
{
    /*
     * reqEnterRoom header
     * datacount : 1 <32bit, 4byte, int>
     * requestType : 4(reqEnterRoom) (32bite, 4byte, int)
     * attr by one element size : 2 <32bit, 4byte, int>
     * attr:
     *    ip length <32bit, 4byte, int>
     *    port length <32bit, 4byte, int>
     */

    try
    {
        int dataCount = 1;
        int requestType = reqEnterRoom;
        int attrSize = 2;
        int ipLength = ip.size();
        int portLength = sizeof(int);

        if (this->_sendByteArray != NULL)
            delete [] this->_sendByteArray;
        this->_sendByteArray = new char[sizeof(int) * 5];
        memcpy(this->_sendByteArray + sizeof(int) * 0, &dataCount, sizeof(int)); // dataCount
        memcpy(this->_sendByteArray + sizeof(int) * 1, &requestType, sizeof(int)); // requestType
        memcpy(this->_sendByteArray + sizeof(int) * 2, &attrSize, sizeof(int)); // attrSize;
        memcpy(this->_sendByteArray + sizeof(int) * 3, &ipLength, sizeof(int));
        memcpy(this->_sendByteArray + sizeof(int) * 4, &portLength, sizeof(int));
        return sizeof(int) * 5;
    }
    catch (const std::bad_alloc &)
    {
        return -1;
    }
}

bool DataHeader::decode()
{
    /*
     * resImage header example
     * datacount : 1 <32bit, 4byte, int>
     * responseType : 1(resImage) (32bite, 4byte, int)
     * attr by one element size :  3
     * attr:
     *     height <32bit, 4byte, int>
     *     width <32bit, 4byte, int>
     *     channels <32bit, 4byte, int>
     */

    /*
     * resRoomList header example
     * datacount : 24 <32bit, 4byte, int>
     * responseType : 2(resRoomList) (32bite, 4byte, int)
     * attr by one element size : 4
     * attr:
     *     first ip length <32bit, 4byte, int>
     *     first port length <32bit, 4byte, int>
     *     first roomName length <32bit, 4byte, int>
     *     first room member count length <32bit, 4byte, int>
     *     second ip length <32bit, 4byte, int>
     *     second port length <32bit, 4byte, int>
     *     second roomName length <32bit, 4byte, int>
     *     second room member count length <32bit, 4byte, int>
     *     ...
     */

    /*
     * resMakeRoom header example
     * datacount : 1 <32bit, 4byte, int>
     * responseType : 3(resMakeRoom) (32bite, 4byte, int)
     * attr by one element size : 1
     * attr:
     *     data length <32bit, 4byte, int>
     */

    this->_attr.clear();
    memcpy(&(this->_dataCount), this->_receiveByteArray + sizeof(int) * 0, sizeof(int));
    memcpy(&(this->_responseType), this->_receiveByteArray + sizeof(int) * 1, sizeof(int));
    memcpy(&(this->_attrSize), this->_receiveByteArray + sizeof(int) * 2, sizeof(int));

    this->_attr.resize(this->_dataCount * this->_attrSize);

    switch(this->_responseType)
    {
    case resImage :
    case resRoomList:
    case resMakeRoom:
        for (int i = 0; i < this->_attr.size(); i++)
            memcpy(&(this->_attr[i]), this->_receiveByteArray + sizeof(int) * (3 + i), sizeof(int));
        return true;
        break;
    }

    return false;
}

