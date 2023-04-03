#include "room_op.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <hiredis/hiredis.h>

#define CREATEROOM_SHELL "./room_create.sh"


std::string CreateRoom()
{

    std::string port;
    char buffer[1024];
    FILE* fp = popen(CREATEROOM_SHELL, "r");
    
    if(NULL == fp)
        return port;

    fread(buffer, 1, sizeof(buffer), fp);
    port.append(buffer);
    pclose(fp);
    return port;
}

std::string server_array[] = {"192.168.88.128"};

std::string create_room(unsigned int _room_index)
{
    static unsigned char cur_select = 0;
    redisContext* redisFD = NULL;
    redisReply* reply = NULL;
    std::string strResult;

    // TODO: 确定创建容器的服务器
    std::string cur_server = server_array[cur_select];
    // 重置服务器指向
    cur_select++;
    cur_select %= sizeof(server_array)/sizeof(server_array[0]);

    // TODO: 连接reids
   redisFD = redisConnect("127.0.0.1", 6379);
    if(NULL == redisFD)
        return "";
    
    // TODO: 发送创建容器的消息给服务器(往redis公共频道create_room(发送要创建容器的服务器IP|房间号))
    freeReplyObject(redisCommand(redisFD, "PUBLISH crateroom %s|%d", cur_server.c_str(), _room_index));

    // TODO: 等待redis公共频道server_recv接收容器服务器创建服务器的状态
    freeReplyObject(redisCommand(redisFD, "SUBSCRIBE server_recv"));
    if(REDIS_OK != redisGetReply(redisFD, (void**)&reply))
    {
        redisFree(redisFD);
        return "";
    }
    strResult.append(reply->element[2]->str);
    freeReplyObject(reply);
    redisFree(redisFD);

    // 拼接返回 IP + Port
    return strResult;
}

unsigned char CheckRoom(unsigned int _roomId)
{
    return 0;
}

unsigned char DelRoom(unsigned int _roomId)
{
    return 0;
}


