#include "response.h"

Response::Response()
//Response::Response(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList)
//    : dataLengthList(dataLengthList)
{
//    this->dataBytesList = new char * [this->dataLengthList.size()]();
//    for (int i = 0; i < this->dataLengthList.size(); i++)
//    {
//        this->dataBytesList[i] = new char[this->dataLengthList[i]];
//        memcpy(this->dataBytesList[i], dataBytesList[i], this->dataLengthList[i]);
//    }
}

Response::~Response()
{
//    initDataBytesList();
}

//void Response::initDataBytesList()
//{
//    if (this->dataBytesList != NULL && this->dataLengthList.size() > 0)
//    {
//        for (int i = 0; i < this->dataLengthList.size(); i++)
//        {
//            if (this->dataBytesList[i] != NULL)
//                delete [] this->dataBytesList[i];
//        }
//        delete [] this->dataBytesList;
//        this->dataBytesList = NULL;
//        this->dataLengthList.resize(0);
//    }
//}

ResImage::ResImage(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
//    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * ResProImage header example
     * receiveCount : 4 <32bit, 4byte, int>
     * responseType : 1(Image) (32bite, 4byte, int)
     * dataSize : 240 * 320 * 3 + 4 + name length + sizeof state <32bit, 4byte, int>
     */
    this->_img = cv::Mat(360, 480, CV_8UC3, (unsigned char *)(dataBytesList[0]));
    memcpy(&this->_number, dataBytesList[1], dataLengthList[1]); //가히
    this->_userNum = std::string(dataBytesList[2], dataLengthList[2]);
    memcpy(&this->_state, dataBytesList[3], dataLengthList[3]);
}

const cv::Mat &ResImage::img()
{
    return this->_img;
}

const int ResImage::number()
{
    return this->_number;
}
const std::string ResImage::userNum()
{
    return this->_userNum;
}

const int ResImage::state()
{
    return this->_state;
}

ResRoomList::ResRoomList(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
//    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resRoomList header example
     * receiveCount : 24 <32bit, 4byte, int>
     * responseType : 2(resRoomList) (32bite, 4byte, int)
     * dataSize : something
     */
    int port = 0;
    int roomMemberCount = 0;
    for (int i = 0; i < dataLengthList.size(); i++)
    {
        switch(i % 4)
        {
        case 0:
            this->_ipList.push_back(std::string(dataBytesList[i], dataLengthList[i]));
            break;
        case 1:
            memcpy(&port, dataBytesList[i], sizeof(int));
            this->_portList.push_back(port);
            break;
        case 2:
            this->_roomNameList.push_back(std::string(dataBytesList[i], dataLengthList[i]));
            break;
        case 3:
            memcpy(&roomMemberCount, dataBytesList[i], sizeof(int));
            this->_roomMemberCountList.push_back(roomMemberCount);
            break;
        }
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
//    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resMakeRoom header example
     * receiveCount : 1 <32bit, 4byte, int>
     * responseType : 3(resMakeRoom) (32bite, 4byte, int)
     * dataSize : 4
     */
    int c;
    memcpy(&c, dataBytesList[0], dataLengthList[0]);

    this->_isMake = (c == 1);
}

bool ResMakeRoom::isMake()
{
    return this->_isMake;
}

ResEnterRoom::ResEnterRoom(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
//    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resEnterRoom header example
     * receiveCount : 1 <32bit, 4byte, int>
     * responseType : 4(ResEnterRoom) (32bite, 4byte, int)
     * dataSize : 4
     */
    int c;
    memcpy(&c, dataBytesList[0], dataLengthList[0]);

    this->_isEnter = (c == 1);
}

bool ResEnterRoom::isEnter()
{
    return this->_isEnter;
}

ResJoinRoom::ResJoinRoom(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
//    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resJoinRoom header example
     * receiveCount : 1 <32bit, 4byte, int>
     * responseType : 5(ResJoinRoom) (32bite, 4byte, int)
     * dataSize : string length
     */
    this->_name = std::string(dataBytesList[0], dataLengthList[0]);
}

const std::string &ResJoinRoom::name()
{
    return this->_name;
}

ResDisjoinRoom::ResDisjoinRoom(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
//    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resDisJoinRoom header example
     * receiveCount : 2 <32bit, 4byte, int>
     * responseType : 6(DisJoinRoom) (32bite, 4byte, int)
     * dataSize : string length + sizeof int
     */

    int c;
    this->_name = std::string(dataBytesList[0], dataLengthList[0]);
    memcpy(&c, dataBytesList[1], dataLengthList[1]);
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

ResLogin::ResLogin(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
//    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resLogin header example
     * receiveCount : 2 <32bit, 4byte, int>
     * responseType : 7(Login) (32bite, 4byte, int)
     * dataSiconze : ment length + name length
     */

    this->_ment = std::string(dataBytesList[0], dataLengthList[0]);
    this->_name = std::string(dataBytesList[1], dataLengthList[1]);
}

const std::string &ResLogin::ment()
{
    return this->_ment;
}

const std::string &ResLogin::name()
{
    return this->_name;
}

//####
ResSignUp::ResSignUp(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
//    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resLogin header example
     * receiveCount : 2 <32bit, 4byte, int>
     * responseType : 8(SignUp) (32bite, 4byte, int)
     * dataSize : sizeof int + ment length
     */

    int c;
    memcpy(&c, dataBytesList[0], dataLengthList[0]);
    this->_isSuccessed = (c == 1);
    this->_ment = std::string(dataBytesList[1], dataLengthList[1]);
}
const bool ResSignUp::isSuccessed()
{
    return this->_isSuccessed;
}

const std::string &ResSignUp::ment()
{
    return this->_ment;
}
//####

ResProImage::ResProImage(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
    : ResImage(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * ResProImage header example
     * receiveCount : 1 <32bit, 4byte, int>
     * responseType : 9(proImage) (32bite, 4byte, int)
     * dataSize : 240 * 320 * 3 <32bit, 4byte, int>
     */
}

ResFirstImage::ResFirstImage(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
    : ResImage(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * ResFirstImage header example
     * receiveCount : 1 <32bit, 4byte, int>
     * responseType : 10(firstImage) (32bite, 4byte, int)
     * dataSize : 240 * 320 * 3 <32bit, 4byte, int>
     */
}

ResSecondImage::ResSecondImage(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
    : ResImage(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * ResSecondImage header example
     * receiveCount : 1 <32bit, 4byte, int>
     * responseType : 11(secondImage) (32bite, 4byte, int)
     * dataSize : 240 * 320 * 3 <32bit, 4byte, int>
     */
}

ResThirdImage::ResThirdImage(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
    : ResImage(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * ResThirdImage header example
     * receiveCount : 1 <32bit, 4byte, int>
     * responseType : 12(thirdImage) (32bite, 4byte, int)
     * dataSize : 240 * 320 * 3 <32bit, 4byte, int>
     */
}

ResForthImage::ResForthImage(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
    : ResImage(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * ResForthImage header example
     * receiveCount : 1 <32bit, 4byte, int>
     * responseType : 13(forthImage) (32bite, 4byte, int)
     * dataSize : 240 * 320 * 3 <32bit, 4byte, int>
     */
}


ResChat::ResChat(const char *headerBytes, const char * const *dataBytesList, int headSize, std::vector<int> &dataLengthList)
//    : Response(headerBytes, dataBytesList, headSize, dataLengthList)
{
    /*
     * resChat header example
     * receiveCount : 2 <32bit, 4byte, int>
     * responseType : 14(Chat) (32bite, 4byte, int)
     * dataSiconze : name length + text length
     */

    this->_name = std::string(dataBytesList[0], dataLengthList[0]);
    this->_text = std::string(dataBytesList[1], dataLengthList[1]);
}

const std::string &ResChat::name()
{
    return this->_name;
}

const std::string &ResChat::text()
{
    return this->_text;
}
