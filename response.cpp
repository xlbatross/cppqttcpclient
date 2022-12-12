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

ResRoomList::ResRoomList(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resRoomList header example
     * datacount : 24 <32bit, 4byte, int>
     * responseType : 2(resRoomList) (32bite, 4byte, int)
     * dataSize : 4
     * dataType:
     *     0 : string
     *     1 : int
     *     0 : string
     *     1 : int
     */
    int memoryLoc = 0;
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
     *     0 : string <32bit, 4byte, int>
     */
    char c;
    memcpy(&c, this->dataBytesList[0], sizeof(char));

    this->_isMake = (c == 1);
}

bool ResMakeRoom::isMake()
{
    return this->_isMake;
}
