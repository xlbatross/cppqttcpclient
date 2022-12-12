#include "response.h"

Response::Response(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList)
    : dataLengthList(dataLengthList)
{
    memcpy(&this->dataSize, headerBytes + sizeof(int) * 2, sizeof(int));
    this->dataTypeList.resize(this->dataSize);
    for (int i = 0; i < this->dataTypeList.size(); i++)
        memcpy(&(this->dataTypeList[i]), headerBytes + sizeof(int) * (3 + i), sizeof(int));

    this->dataBytesList = new char * [this->dataLengthList.size()]();
    for (int i = 0; i < this->dataLengthList.size(); i++)
    {
        this->dataBytesList[i] = new char[this->dataLengthList[i]];
        memcpy(this->dataBytesList[i], dataBytesList[i], this->dataLengthList[i]);
    }
}

Response::~Response()
{
    initDataBytesList();
}

void Response::initDataBytesList()
{
    if (this->dataBytesList != NULL && this->dataLengthList.size() > 0)
    {
        for (int i = 0; i < this->dataLengthList.size(); i++)
        {
            if (this->dataBytesList[i] != NULL)
                delete [] this->dataBytesList[i];
        }
        delete [] this->dataBytesList;
        this->dataBytesList = NULL;
        this->dataLengthList.resize(0);
    }
}

ResImage::ResImage(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resImage header example
     * receiveCount : 2 <32bit, 4byte, int>
     * responseType : 1(resRoomList) (32bite, 4byte, int)
     * dataSize : 2
     * dataType:
     *     2 : OpenCVImage
     *     1 : int
     */
    this->_img = cv::Mat(240, 320, CV_8UC3, (unsigned char *)(dataBytesList[0]));
    memcpy(&_number, dataBytesList[1], sizeof(int));
}

const cv::Mat &ResImage::img()
{
    return this->_img;
}

const int ResImage::number()
{
    return this->_number;
}

ResRoomList::ResRoomList(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resRoomList header example
     * receiveCount : 24 <32bit, 4byte, int>
     * responseType : 2(resRoomList) (32bite, 4byte, int)
     * dataSize : 4
     * dataType:
     *     0 : string
     *     1 : int
     *     0 : string
     *     1 : int
     */
    int port = 0;
    int roomMemberCount = 0;
    for (int i = 0; i < this->dataLengthList.size(); i++)
    {
        switch(i % dataSize)
        {
        case 0:
            this->_ipList.push_back(std::string(this->dataBytesList[i], this->dataLengthList[i]));
            break;
        case 1:
            memcpy(&port, this->dataBytesList[i], sizeof(int));
            this->_portList.push_back(port);
            break;
        case 2:
            this->_roomNameList.push_back(std::string(this->dataBytesList[i], this->dataLengthList[i]));
            break;
        case 3:
            memcpy(&roomMemberCount, this->dataBytesList[i], sizeof(int));
            this->_roomMemberCountList.push_back(roomMemberCount);
            break;
        }
//        std::cout << this->_ipList[i] << std::endl;
//        std::cout << this->_portList[i] << std::endl;
//        std::cout << this->_roomNameList[i] << std::endl;
//        std::cout << this->_roomMemberCountList[i] << std::endl;
    }
}

const std::vector<std::string> &ResRoomList::ipList()
{
    return this->_ipList;
}

const std::vector<int> &ResRoomList::portList()
{
    return this->_portList;
}

const std::vector<std::string> &ResRoomList::roomNameList()
{
    return this->_roomNameList;
}

const std::vector<int> &ResRoomList::roomMemberCountList()
{
    return this->_roomMemberCountList;
}

ResMakeRoom::ResMakeRoom(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resMakeRoom header example
     * receiveCount : 1 <32bit, 4byte, int>
     * responseType : 3(resMakeRoom) (32bite, 4byte, int)
     * dataSize : 1
     * dataType:
     *     1 : Int <32bit, 4byte, int>
     */
    int c;
    memcpy(&c, this->dataBytesList[0], sizeof(int));

    this->_isMake = (c == 1);
}

bool ResMakeRoom::isMake()
{
    return this->_isMake;
}

ResEnterRoom::ResEnterRoom(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resEnterRoom header example
     * receiveCount : 1 <32bit, 4byte, int>
     * responseType : 4(ResEnterRoom) (32bite, 4byte, int)
     * dataSize : 1
     * dataType:
     *     1 : Int <32bit, 4byte, int>
     */
    int c;
    memcpy(&c, this->dataBytesList[0], sizeof(int));

    this->_isEnter = (c == 1);
}

bool ResEnterRoom::isEnter()
{
    return this->_isEnter;
}

ResJoinRoom::ResJoinRoom(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resJoinRoom header example
     * receiveCount : 1 <32bit, 4byte, int>
     * responseType : 5(ResJoinRoom) (32bite, 4byte, int)
     * dataSize : 1
     * dataType:
     *     0 : string <32bit, 4byte, int>
     */
    this->_name = std::string(this->dataBytesList[0], this->dataLengthList[0]);
}

const std::string &ResJoinRoom::name()
{
    return this->_name;
}

ResDisjoinRoom::ResDisjoinRoom(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resDisJoinRoom header example
     * receiveCount : 2 <32bit, 4byte, int>
     * responseType : 6(DisJoinRoom) (32bite, 4byte, int)
     * dataSize : 2
     * dataType:
     *     0 : string <32bit, 4byte, int>
     *     1 : int <32bit, 4byte, int>
     */
    int c;
    this->_name = std::string(this->dataBytesList[0], this->dataLengthList[0]);
    memcpy(&c, this->dataBytesList[1], sizeof(int));
    this->_isProfessorOut = (c == 1);
}

const std::string &ResDisjoinRoom::name()
{
    return this->_name;
}

const bool ResDisjoinRoom::isProfessorOut()
{
    return this->_isProfessorOut;
}


