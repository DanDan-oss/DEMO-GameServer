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

void InitSessiobContext();	// ��ʼ������Ϊ��̨�Ự����


redisContext* InitRedisContext();	// ��ʼ��redis����
void InitRedisAsyncContext();
void redisAsyncCallBack(struct redisAsyncContext*, void*, void*);

#endif // !MAIN_H
