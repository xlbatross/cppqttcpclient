#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class Response
{
public:
    enum Type{Image = 1, RoomList, MakeRoom, EnterRoom, JoinRoom, DisjoinRoom};
    enum Data{String = 0, Int, OpenCVImage};
    Response(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
    ~Response();

    void initDataBytesList();

protected:
    char ** dataBytesList;
    std::vector<int> dataLengthList;

    int dataSize;
    std::vector<int> dataTypeList;
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


#endif // RESPONSE_H
