#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class Response
{
public:
    enum Type{Image = 1, RoomList, MakeRoom, EnterRoom, JoinRoom, DisjoinRoom, Login, SignUp,
              ProImage, FirstImage, SecondImage, ThirdImage, ForthImage};
//    Response(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
    Response();
    ~Response();

//    void initDataBytesList();

//protected:
//    char ** dataBytesList;
//    std::vector<int> dataLengthList;
};

class ResImage : public Response
{
public:
    ResImage(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);

    const cv::Mat & img();
    const int number();

private:
    cv::Mat _img;
    int _number;
};

class ResRoomList : public Response
{
public:
    ResRoomList(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);

    const std::vector<std::string> & ipList();
    const std::vector<int> & portList();
    const std::vector<std::string> & roomNameList();
    const std::vector<int> & roomMemberCountList();

private:
    std::vector<std::string> _ipList;
    std::vector<int> _portList;
    std::vector<std::string> _roomNameList;
    std::vector<int> _roomMemberCountList;
};

class ResMakeRoom : public Response
{
public:
    ResMakeRoom(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
    bool isMake();

private:
    bool _isMake;
};

class ResEnterRoom : public Response
{
public:
    ResEnterRoom(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
    bool isEnter();

private:
    bool _isEnter;
};

class ResJoinRoom : public Response
{
public:
    ResJoinRoom(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
    const std::string & name();

private:
    std::string _name;
};

class ResDisjoinRoom : public Response
{
public:
    ResDisjoinRoom(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
    const std::string & name();
    const bool isProfessorOut();

private:
    std::string _name;
    bool _isProfessorOut;
};

class ResLogin : public Response
{
public:
    ResLogin(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
    const bool isSuccessed();
    const std::string & ment();

private:
    bool _isSuccessed;
    std::string _ment;
};

//####
class ResSignUp : public Response
{
public:
    ResSignUp(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
    const bool isSuccessed();
    const std::string & ment();

private:
    bool _isSuccessed;
    std::string _ment;
};
//####


class ResProImage : public ResImage
{
public:
    ResProImage(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
};

class ResFirstImage : public ResImage
{
public:
    ResFirstImage(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
};

class ResSecondImage : public ResImage
{
public:
    ResSecondImage(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
};

class ResThirdImage : public ResImage
{
public:
    ResThirdImage(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);

};

class ResForthImage : public ResImage
{
public:
    ResForthImage(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
};


#endif // RESPONSE_H
