#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class Request
{
public:
    enum Type{sendImage = 1, RoomList, MakeRoom, EnterRoom};
    enum Data{String = 0, Int, Image};
    Request();
    ~Request();

    const char * headerBytes();
    const char * const * dataBytesList();
    const int headerSize();
    const std::vector<int> & dataLengthList();

    void initHeaderBytes();
    void initDataBytesList();

protected:
    char * _headerBytes;
    char ** _dataBytesList;
    int _headerSize;
    std::vector<int> _dataLengthList;
};

class ReqRoomList : public Request
{
public:
    ReqRoomList();
};

class ReqMakeRoom : public Request
{
public:
    ReqMakeRoom(std::string roomName);
};

class ReqEnterRoom : public Request
{
public:
    ReqEnterRoom(std::string ip, int port);
};


#endif // REQUEST_H
