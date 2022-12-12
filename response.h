#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <vector>

class Response
{
public:
    enum Type{sendImage = 1, RoomList, MakeRoom};
    enum Data{String = 0, Int, Image};
    Response(const char * headerBytes, const char * const * dataBytesList, int headSize, std::vector<int> & dataLengthList);
    ~Response();

    void initDataBytesList();

protected:
    char ** dataBytesList;
    std::vector<int> dataLengthList;

    int dataSize;
    std::vector<int> dataTypeList;
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

#endif // RESPONSE_H
