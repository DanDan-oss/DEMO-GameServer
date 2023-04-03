//#include <cstdio>
#include <iostream>
#include "GameNetwork.h"
#include "msg.pb.h"
#include "GameMSG.h"
#include "GameWorld.h"
#include <list>
#include "main.h"

#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>


#define  SERVER_DEBUG
#define  SERVER_PORT 8888
#define  REDIS_ADDR "192.168.88.128"
#define  REDIS_PORT 6379


void testSer()
{
	pb::SyncPid pid;
	pid.set_playid(100);
	pid.set_username("hahaha");
	GameMSG game(GameMSG::MSG_TYPE_LOGIN_ID_NAME, &pid);

	// 0a0000000100000008641206686168616861 
	std::string str;
	str = game.Serialize();
	std::cout << std::hex << str.size() << " " << htonl(str.size()) << " " << htonl(GameMSG::MSG_TYPE_LOGIN_ID_NAME) << std::endl;
	std::cout << std::dec << "===========================" << std::endl;

	for (char byte : str)
	{
		printf(" %02X ", byte);
	}
	puts("");
	std::cout << std::endl << "===========================" << std::endl;
	GameMSG cGame(GameMSG::MSG_TYPE_LOGIN_ID_NAME, str);

	std::cout << dynamic_cast<pb::SyncPid*>(cGame.GetMessagePoint())->playid() << std::endl;
	std::cout << dynamic_cast<pb::SyncPid*>(cGame.GetMessagePoint())->username() << std::endl;
}

void testRedis()
{

	redisContext* redis = redisConnect("127.0.0.1", 6379);
	if (redis == NULL || redis->err) {
		if (redis)
		{
			printf("Error: %s\n", redis->errstr);
			// handle error
		}
		else
			printf("Can't allocate redis context\n");
	}
	redisReply* ret = (redisReply*)redisCommand(redis, "Get ccc");
	std::cout << ret->len << " :" << ret->str << std::endl;
	freeReplyObject(ret);
	redisFree(redis);
}

void testAoi()
{
	WorldAOI world(20, 200, 50, 230, 6, 6);
	std::string s1 = "ZhangSan";
	std::string s2 = "LiSi";
	std::string s3 = "WangErMaZi";

	//Player p2(68, 90, s3);
	//world.AddPlayer(*(new Player(60, 107, s1)));
	//world.AddPlayer(*(new Player(91, 118, s2)));
	//world.AddPlayer(*(new Player(61, 90, s3)));

	//std::list<Player*> list1 = world.GetSraPlayers(&p2);

	//for (Player* temp : list1)
	//	std::cout << "Player[" << temp->GetName() << "] X=" << temp->GetX() << " Y=" << temp->GetY() << std::endl;

	//std::list<Player*> list2 = world.GetSraPlayers(&p2);
	//std::cout << " =============" << std::endl;
	//for (Player* temp : list2)
	//	std::cout << "Player[" << temp->GetName() << "] X=" << temp->GetX() << " Y=" << temp->GetY() << std::endl;
	//std::cout << " =============" << std::endl;
	return;
}


int main()
{
	InitRedisAsyncContext();
	//InitSessiobContext();
	//ZinxKernel::ZinxKernelInit();
	//ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(SERVER_PORT, new GameTcpFact())));
	//ZinxKernel::Zinx_Run();
	//ZinxKernel::ZinxKernelFini();
	//DbgPrintA("gamelog", "Gmae Player is zeor, process exit!");
	return 0;
}

static redisContext* redis = InitRedisContext();
static int MAXLEN = 4096;

redisContext* InitRedisContext()
{
	redisContext* redis = redisConnect(REDIS_ADDR, REDIS_PORT);
	if (redis == NULL || redis->err) {
		if (redis)
		{
			printf("connect redis Error: %s\n", redis->errstr);
			// handle error
		}
		else
			printf("Can't allocate redis context\n");
		return nullptr;
	}
	return redis;
}

void InitSessiobContext()
{
	pid_t sid = 0;			// 会话ID
	pid_t pid = fork();		// 进程ID
	int file_fd = 0;

	//TODO: 创建子进程,父进程退出
	if (0 != pid)
	{
		DbgPrintA("gamelog", "fork pid: %d  process exit!", pid);
		//std::cout << "fork pid:" << pid << " process exit!" << std::endl;
		exit(0);
	}

	//TODO: 子进程提升为会话
	sid = setsid();
	if (0 == sid)
	{
		DbgPrintA("gamelog", "setsid sid: %d  process exit!", sid);
		//std::cout << "setsid sid:" << sid << " process exit!" << std::endl;
		exit(0);
	}

	//TODO: 修改当前进程运行目录,防止不安全目录丢失
	// 
	// TODO: 重定向读写错误文件描述符
	file_fd = open("/dev/null", O_RDWR);
	if (-1 == file_fd)
	{
		DbgPrintA("gamelog", "open \"/dev/null\" error: %d  process exit!", file_fd);
		//std::cout << "open \"/dev/null\" error:" << " process exit!" << std::endl;
		exit(0);
	}
	dup2(file_fd, STDIN_FILENO);
	dup2(file_fd, STDOUT_FILENO);
	dup2(file_fd, STDERR_FILENO);
	// TODO: 子进程再创建子进程,然后当前子进程做监视进程,子子进程运行业务逻辑
	while (true)
	{
		int status = 0;
		pid_t pid = fork();
		if (0 == pid)
		{
			DbgPrintA("gamelog", "proce create process pid=%d", getpid());
			return;
		}

		pid = wait(&status);
		if (WIFEXITED(status))
		{
			// 子进程正常退出
			DbgPrintA("gamelog", "proce exit status: %d pid=%d", WEXITSTATUS(status), pid);
			//std::cout << "proce exit status: " << WEXITSTATUS(status);
			return;
		}
		if (WIFSIGNALED(status))
		{
			// 子进程被信号杀手
			DbgPrintA("gamelog", "Kill proce signal: %d pid=%d", WTERMSIG(status), pid);
			exit(0);
		}
			
		//std::cout << "Kill proce signal: " << WTERMSIG(status);
		continue;
	}

	return;
}

void  OutputDebugStringFA(const char* desc, const char* log, ...)
{
	char* strBuffer = (char*)malloc(sizeof(char*) * MAXLEN);
	va_list vArgList;

	va_start(vArgList, log);
	vsnprintf(strBuffer, MAXLEN - 1, log, vArgList);
	va_end(vArgList);

	if (redis != NULL)
		freeReplyObject((redisReply*)redisCommand(redis, "RPUSH %s %s\0", desc, strBuffer));
	else
		std::cout << strBuffer << std::endl;

	free(strBuffer);
	return;
}

void OutputDebugStringFW(const char* desc, const wchar_t* log, ...)
{
	wchar_t* strBuffer = (wchar_t*)malloc(sizeof(wchar_t*) * MAXLEN);
	va_list vArgList;

	va_start(vArgList, log);
	vswprintf(strBuffer, MAXLEN - 1, log, vArgList);
	va_end(vArgList);
	if (redis != NULL)
		freeReplyObject((redisReply*)redisCommand(redis, "RPUSH %s %s \0", desc, strBuffer));
	else
		std::cout << strBuffer << std::endl;
	free(strBuffer);
	return;
}

void redisAsyncCallBack(struct redisAsyncContext* c, void* reply, void* privdata)
{
	redisReply* pReply = (redisReply*)reply;
	std::string msgType = pReply->element[0]->str;
	if (0 == msgType.compare("subscribe"))
	{	// 订阅消息
		std::cout << "type:" << msgType.c_str() << std::endl;
		std::cout<< "message from channel:" << pReply->element[1]->str << std::endl;
		std::cout << "message connect number:" << pReply->element[2]->integer << std::endl;
		return;
	}
	if (0 == msgType.compare("message"))
	{
		std::cout << "type:" << msgType.c_str() << std::endl;
		std::cout << "message from channel:" << pReply->element[1]->str << std::endl;
		std::cout << "message data:" << pReply->element[2]->str << std::endl;
		return;
	}
	return;
}

void InitRedisAsyncContext()
{
	int result = 0;
	struct event_base* base = event_base_new();
	if (NULL == base)
		return;
	redisAsyncContext* asyncRedis = redisAsyncConnect("127.0.0.1", 6379);	// 异步链接
	if (NULL == asyncRedis)
	{
		event_base_free(base);
		return;
	}
	result = redisLibeventAttach(asyncRedis, base);		// 关联libevent和reids链接
	result = redisAsyncCommand(asyncRedis, redisAsyncCallBack, NULL, "subscribe 1");	// 订阅频道1
	event_base_dispatch(base);	// 开始事件循环
	return;
}


