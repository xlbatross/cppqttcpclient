#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class Request
{
public:
    enum Type{Image = 1, RoomList, MakeRoom};
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


#endif // REQUEST_H
