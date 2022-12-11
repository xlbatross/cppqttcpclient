#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <vector>

class Response
{
public:
    enum Type{Image = 1, RoomList, MakeRoom};
    Response(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
    ~Response();

    void initDataBytesList();

protected:
    char ** dataBytesList;
    std::vector<int> dataLengthList;

    int attrSize;
    std::vector<int> attrList;
};

class ResRoomList : public Response
{
public:
    ResRoomList(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);

private:
    std::vector<std::string> ipList;
    std::vector<int> portList;
    std::vector<std::string> roomNameList;
    std::vector<int> roomMemberCountList;
};

class ResMakeRoom : public Response
{
public:
    ResMakeRoom(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);

    bool isMake();

private:
    bool _isMake;
};

#endif // RESPONSE_H
