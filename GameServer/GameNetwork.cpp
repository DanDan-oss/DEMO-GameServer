#include "GameNetwork.h"
#include "GameProtocol.h"

#define  GAME_TIMEOUT_EXIT 5

// =============================
// 时间轮
void ExitTimer::Proc()
{
	ZinxKernel::Zinx_Exit();
}


int ExitTimer::GetTimeSec()
{
	return GAME_TIMEOUT_EXIT;
}

// ==============================
// TCP消息通道层， socket

static ExitTimer g_exit_time;

GameChanelData::GameChanelData(int _fd)
	: ZinxTcpData(_fd)
{

	//if (ZinxKernel::Zinx_GetAllRole().size() <= 0)
	//	TimerManage::GetInstance()->DelTask(&g_exit_time);
}


GameChanelData::GameChanelData(int _fd, GameProtocol* _proto)
	: ZinxTcpData(_fd), m_proto(_proto)
{
	_proto->SetChanel(this);

	//if (ZinxKernel::Zinx_GetAllRole().size() <= 0)
	//	TimerManage::GetInstance()->DelTask(&g_exit_time);
}

GameChanelData::~GameChanelData()
{
	if (this->m_proto)
	{
		ZinxKernel::Zinx_Del_Proto(*(this->m_proto));
		delete this->m_proto;
	}

	if (ZinxKernel::Zinx_GetAllRole().size() < 1)
		ZinxKernel::Zinx_Exit();
	//if (ZinxKernel::Zinx_GetAllRole().size() < 1)
	//	TimerManage::GetInstance()->AddTask(&g_exit_time);
}

AZinxHandler* GameChanelData::GetInputNextStage(BytesMsg& _oInput)
{
	// 设置当前通道对象的下一个消息处理对象为绑定的协议对象
	return this->m_proto;
}

void GameChanelData::SetProtocol(GameProtocol* _proto)
{
	this->m_proto = _proto;
}

// ==============================
// 套接字工厂类
ZinxTcpData* GameTcpFact::CreateTcpDataChannel(int _fd)
{
	GameChanelData* pChanel = new GameChanelData(_fd);
	GameProtocol* pProto = new GameProtocol(pChanel);
	pChanel->SetProtocol(pProto);

	// 将协议对象添加到协议链表,方便以后查找
	ZinxKernel::Zinx_Add_Channel(*pChanel);
	ZinxKernel::Zinx_Add_Proto(*pProto);
	return pChanel;
}


