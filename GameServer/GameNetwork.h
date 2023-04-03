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
	// ͨ�� TimerOutProc �̳�
	virtual void Proc() override;
	virtual int GetTimeSec() override;
};

/* TCP��Ϣͨ���㣬 socket 
ͨ���㴴��ʱ����Э���,����д���Э�������Э��㻥���(�������²����ϲ㴫)
Э�����ά��, ͨ�����ͷ�ʱ,�Զ��ͷŰ󶨵�Э��� */
class GameChanelData : public ZinxTcpData
{
	friend class GameProtocol;
public:
	// ͨ�� ZinxTcpData �̳�
	virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;//��ȡ��Ϣ��һ���������

	GameChanelData(int _fd);
	GameChanelData(int _fd, GameProtocol* _proto);	// ���봫���_proto�Զ���
	virtual  ~GameChanelData();

	void SetProtocol(GameProtocol* _proto);		/* �󶨵�ǰͨ������� Э����� */

private:
	GameProtocol* m_proto = nullptr;	// Э�����
};

/* �׽��ֹ�����*/
class GameTcpFact : public IZinxTcpConnFact
{
	// ͨ�� IZinxTcpConnFact �̳�
	virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};

#endif // !GAME_NETWORK_H




