#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class Request
{
public:
<<<<<<< HEAD
    enum Type{Image = 1, RoomList, MakeRoom, EnterRoom, LeaveRoom, Login, SignUp};
=======
    enum Type{Image = 1, RoomList, MakeRoom, EnterRoom, LeaveRoom};
>>>>>>> eb5fbce6a906683b9992b73f22f876f3e0feff9b
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

class ReqImage : public Request
{
public:
    ReqImage(const cv::Mat & img);
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

class ReqLeaveRoom : public Request
{
public:
    ReqLeaveRoom();
};

<<<<<<< HEAD
class ReqLogin : public Request
{
public:
    ReqLogin(std::string num, std::string pw);
};

class ReqSignUp : public Request
{
public:
    ReqSignUp(std::string name, std::string num, std::string pw, std::string cate );
};
=======
>>>>>>> eb5fbce6a906683b9992b73f22f876f3e0feff9b

#endif // REQUEST_H
