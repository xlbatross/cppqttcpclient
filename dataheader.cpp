#include "dataheader.h"

DataHeader::DataHeader()
{

}

DataHeader::~DataHeader()
{
    delete [] this->_sendCharArray;
}

const char *DataHeader::sendCharArray()
{
    return this->_sendCharArray;
}

QByteArray &DataHeader::receiveByteArray()
{
    return this->_receiveByteArray;
}

qint32 DataHeader::dataType()
{
    return this->_dataType;
}

const QVector<qint32> &DataHeader::attr()
{
    return this->_attr;
}

qint32 DataHeader::encode(cv::Mat const & data)
{
    /*
     * header template
     * datatype : 1(image) (32bite, 4byte, int)
     * attr:
     *     height <32bit, 4byte, int>
     *     width <32bit, 4byte, int>
     *     channels <32bit, 4byte, int>
     */

    try
    {
        qint32 dataType = 1; // image
        qint32 height = data.rows;
        qint32 width = data.cols;
        qint32 channels = data.channels();

        delete [] this->_sendCharArray;
        this->_sendCharArray = new char[sizeof(qint32) * 4];
        // 이유는 모르겠지만, memcpy는 littleEndian 형식으로 복사된다.
        // 서버에서 헤더를 디코딩할 때는 각각 영역에 맞는 바이트를 little Endian으로 읽어야 한다.
        memcpy(this->_sendCharArray + sizeof(qint32) * 0, &dataType, sizeof(qint32)); // datatype
        memcpy(this->_sendCharArray + sizeof(qint32) * 1, &height, sizeof(qint32)); // height
        memcpy(this->_sendCharArray + sizeof(qint32) * 2, &width, sizeof(qint32)); // width
        memcpy(this->_sendCharArray + sizeof(qint32) * 3, &channels, sizeof(qint32)); // channels
        return sizeof(qint32) * 4;
    }
    catch (const std::bad_alloc &)
    {
        return -1;
    }
}

bool DataHeader::decode()
{

}

