#ifndef MAIN_H
#define  MAIN_H

#define  _DEBUGOUT

#ifdef _DEBUGOUT
#define DbgPrintA OutputDebugStringFA
#define DbgPrintW OutputDebugStringFW
#else
#define DbgPrintA
#define DbgPrintW
#endif


#include <hiredis/adapters/libevent.h>
#include <hiredis/hiredis.h>

void OutputDebugStringFA(const char* desc,const char* log, ...);
void OutputDebugStringFW(const char* desc, const wchar_t* log, ...);

void InitSessiobContext();	// 初始化程序为后台会话服务


redisContext* InitRedisContext();	// 初始化redis环境
void InitRedisAsyncContext();
void redisAsyncCallBack(struct redisAsyncContext*, void*, void*);

#endif // !MAIN_H
