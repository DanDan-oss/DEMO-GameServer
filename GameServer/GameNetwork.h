#ifndef GAME_NETWORK_H
#define GAME_NETWORK_H

#include<ZinxTCP.h>
#include "ZinxTimer.h"

class ExitTimer;
class GameChanelData;
class GameProtocol;
class GameTcpFact;

class ExitTimer : public TimerOutProc
{
	// 通过 TimerOutProc 继承
	virtual void Proc() override;
	virtual int GetTimeSec() override;
};

/* TCP消息通道层， socket 
通道层创建时创建协议层,如果有传入协议层则与协议层互相绑定(数据由下层往上层传)
协议层由维护, 通道层释放时,自动释放绑定的协议层 */
class GameChanelData : public ZinxTcpData
{
	friend class GameProtocol;
public:
	// 通过 ZinxTcpData 继承
	virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;//获取消息下一个处理对象

	GameChanelData(int _fd);
	GameChanelData(int _fd, GameProtocol* _proto);	// 会与传入的_proto自动绑定
	virtual  ~GameChanelData();

	void SetProtocol(GameProtocol* _proto);		/* 绑定当前通道对象的 协议对象 */

private:
	GameProtocol* m_proto = nullptr;	// 协议对象
};

/* 套接字工厂类*/
class GameTcpFact : public IZinxTcpConnFact
{
	// 通过 IZinxTcpConnFact 继承
	virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};

#endif // !GAME_NETWORK_H




