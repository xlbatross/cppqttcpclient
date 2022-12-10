#include "dataheader.h"

DataHeader::DataHeader()
{
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

long DataHeader::responseType()
{
    return this->_responseType;
}

long DataHeader::dataCount()
{
    return this->_dataCount;
}

const std::vector<long> &DataHeader::attr()
{
    return this->_attr;
}

long DataHeader::encode(cv::Mat const & data)
{
    /*
     * send header template
     * datacount : 1 <32bit, 4byte, int>
     * requestType : 1(image) (32bite, 4byte, int)
     * attr:
     *     height <32bit, 4byte, int>
     *     width <32bit, 4byte, int>
     *     channels <32bit, 4byte, int>
     */

    try
    {
        long dataCount = 1;
        long requestType = reqImage; // image
        long height = data.rows;
        long width = data.cols;
        long channels = data.channels();

        if (this->_sendByteArray != NULL)
            delete [] this->_sendByteArray;
        this->_sendByteArray = new char[sizeof(long) * 5];
        memcpy(this->_sendByteArray + sizeof(long) * 0, &dataCount, sizeof(long)); // dataCount
        memcpy(this->_sendByteArray + sizeof(long) * 1, &requestType, sizeof(long)); // requestType
        memcpy(this->_sendByteArray + sizeof(long) * 2, &height, sizeof(long)); // height
        memcpy(this->_sendByteArray + sizeof(long) * 3, &width, sizeof(long)); // width
        memcpy(this->_sendByteArray + sizeof(long) * 4, &channels, sizeof(long)); // channels
        return sizeof(long) * 5;
    }
    catch (const std::bad_alloc &)
    {
        return -1;
    }
}

bool DataHeader::decode()
{
//    std::cout << "in" << std::endl;
//    for (int i = 0; i < 4; i++)
//    {
//        std::cout << (int)(*(this->_receiveByteArray + i)) << " ";
//    }
//    std::cout << std::endl;
    this->_attr.clear();
    memcpy(&(this->_dataCount), this->_receiveByteArray + sizeof(long) * 0, sizeof(long));
    memcpy(&(this->_responseType), this->_receiveByteArray + sizeof(long) * 1, sizeof(long));

    switch(this->_responseType)
    {
    /*
     * receive header template
     * datacount : 1 <32bit, 4byte, int>
     * responseType : 1(image) (32bite, 4byte, int)
     * attr:
     *     height <32bit, 4byte, int>
     *     width <32bit, 4byte, int>
     *     channels <32bit, 4byte, int>
     */
    case resImage :
        this->_attr.resize(3);
        memcpy(&_attr[0], this->_receiveByteArray + sizeof(long) * 2, sizeof(long)); // height
        memcpy(&_attr[1], this->_receiveByteArray + sizeof(long) * 3, sizeof(long)); // width
        memcpy(&_attr[2], this->_receiveByteArray + sizeof(long) * 4, sizeof(long)); // channels
        return true;
        break;
    }
    return false;
}

