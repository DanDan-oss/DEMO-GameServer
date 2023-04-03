#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "user_op.h"

#define ADDUSER_SHELL "./user_add.sh"
#define CHECKUSER_SHELL "./user_check.sh"
#define DELUSER_SHELL "./user_del.sh"


// 存在返回1 没存在返回0
unsigned char CheckUser(const char* _name, const char* _password_dgst)
{
	int bResult = 0;

	if(0 == fork())
	{
		// TODO: 子进程执行脚本
		execlp(CHECKUSER_SHELL, CHECKUSER_SHELL, _name, _password_dgst, NULL);
	}
	// TODO: 父进程等待子进程执行shell后回收子进程
	wait(&bResult);
	if(0 == bResult)
		return 1;
	return 0;
}

void AddUser(const char* _name, const char* _password)
{
	char buff[1024] = {0};
	if(0 == CheckUser(_name, NULL))
	{
		sprintf(buff, "%s %s %s", ADDUSER_SHELL,_name, _password);
		system(buff);
	}
	return;
}

unsigned char DelUser(const char* _name)
{
	char buff[1024] = {0};
	if(1 == CheckUser(_name, NULL))
	{
		sprintf(buff, "%s %s", DELUSER_SHELL, _name);
		system(buff);
	}
	return 1;
}

unsigned char ChangePssword(const char* _name, const char* _password)
{
	DelUser(_name);
	AddUser(_name, _password);
	return 1;
}

/*
echo '{"username":"1234","password":"81dc9bdb52d04dc20036dbd8313ed055","requetype":"room_crate"}' | CONTENT_LENGTH=90 ./room.out
*/
