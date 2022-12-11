#include "response.h"

Response::Response(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList)
    : dataLengthList(dataLengthList)
{
    memcpy(&this->attrSize, headerBytes + sizeof(int) * 2, sizeof(int));
    this->attrList.resize(this->attrSize * this->dataLengthList.size());
    for (int i = 0; i < this->attrList.size(); i++)
        memcpy(&(this->attrList[i]), headerBytes + sizeof(int) * (3 + i), sizeof(int));

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
    int memoryLoc = 0;
    int port = 0;
    int roomMemberCount = 0;
    for (int i = 0; i < this->dataLengthList.size(); i++)
    {
        memoryLoc = 0;

        this->_ipList.push_back(std::string(dataBytesList[i] + memoryLoc, this->attrList[this->attrSize * i + 0]));
        memoryLoc += this->attrList[this->attrSize * i + 0];

        memcpy(&port, dataBytesList[i] + memoryLoc, this->attrList[this->attrSize * i + 1]);
        this->_portList.push_back(port);
        memoryLoc += this->attrList[this->attrSize * i + 1];

        this->_roomNameList.push_back(std::string(dataBytesList[i] + memoryLoc, this->attrList[this->attrSize * i + 2]));
        memoryLoc += this->attrList[this->attrSize * i + 2];

        memcpy(&roomMemberCount, dataBytesList[i] + memoryLoc, this->attrList[this->attrSize * i + 3]);
        this->_roomMemberCountList.push_back(roomMemberCount);

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
    char c;
    memcpy(&c, this->dataBytesList[0], this->attrList[0]);

    this->_isMake = (c == 1);
}

bool ResMakeRoom::isMake()
{
    return this->_isMake;
}
