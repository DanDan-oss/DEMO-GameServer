#include <stdlib.h>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
#include <string>

#include <iostream>

#define CURENT_ADDR "192.168.88.128"    // 不同机器需要重设服务器当前IP地址
#define SERVER_ADDR "192.168.88.128"
#define SERVER_PORT 6379

#define CREATEROOM_SHELL "./room_create.sh "


void crateroom_callback(struct redisAsyncContext* c, void* reply, void* privdata);

int main()
{
    struct event_base* base = event_base_new();
    redisAsyncContext* asyncRedis = redisAsyncConnect(SERVER_ADDR, SERVER_PORT);
    if(NULL== asyncRedis)
    {
        return -1;
    }
    redisLibeventAttach(asyncRedis, base);
    redisAsyncCommand(asyncRedis, crateroom_callback, NULL, "SUBSCRIBE crateroom");
    event_base_dispatch(base);


    redisAsyncDisconnect(asyncRedis);
    event_base_free(base);
    return 0;
}

void crateroom_callback(struct redisAsyncContext* c, void* reply, void* privdata)
{

    FILE* pfd = NULL;
    std::string strResult;
    redisReply* pReply = (redisReply*)reply;
    std::string msgType = pReply->element[0]->str;
    


    if (0 != msgType.compare("message"))
	{	// 不是订阅之后其他服务器发过来的消息
		return;
	}

    // 消息结构 "IP地址|房间号"
    std::string msgValue = pReply->element[2]->str;
    int carveLen = msgValue.find("|");
    std::string msgIP = msgValue.substr(0, carveLen-1);
    
    if(0 != msgIP.compare(CURENT_ADDR))
    { // 判断不是当前服务器的消息
        return;
    }
    std::string roomIndex = msgValue.substr(carveLen+1, msgValue.size()-1);
    std::string cmdRun = (CREATEROOM_SHELL)+ roomIndex;
    pfd = popen(cmdRun.data(), "r");
    if(NULL== pfd)
        return;
    
    //返回 "端口\n"
    char buffer[1024];
    fread(buffer, 1, sizeof(buffer), pfd);
    strResult.append(buffer);
    strResult.pop_back();
    pclose(pfd);

    //std::cout<<"strResult: " << strResult<<std::endl;
    redisContext* redisFD=  redisConnect(SERVER_ADDR, SERVER_PORT);
    if(NULL == redisFD)
        return;

    // 往fastcgi等待的reids频道写入当前创建的服务器
    freeReplyObject(redisCommand(redisFD, "PUBLISH server_recv %s|%s:%s", roomIndex.data(), msgIP.data(), strResult.data()));

    // 往reids 服务器list中写入当前服务器的信息
    //freeReplyObject(redisCommand(redisFD, "RPUSH roomindex %s:%s", msgIP.data(), strResult.data()));

    redisFree(redisFD);
    return;
}
