#ifndef GAME_PROTOCOL_H
#define GAME_PROTOCOL_H

#include <zinx.h>
class GameChanelData;
class GameRole;

/* Э���
������ͨ������������������߰�Э������(�������²����ϲ㴫),
����ͨ�����Э������֮��Ĺ�ϵ��ͨ����ά��.
ҵ�����Э���ά��,Э����ͷ�ʱ��ҵ����ͷ� */
class GameProtocol : public Iprotocol
{
	friend class GameChanelData;
	friend class GameRole;

public:
	// ͨ�� Iprotocol �̳�
	virtual UserData* raw2request(std::string _szInput) override;
	virtual std::string* response2raw(UserData& _oUserData) override;
	virtual Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
	virtual Ichannel* GetMsgSender(BytesMsg& _oBytes) override;

	GameProtocol();
	GameProtocol(GameChanelData* _chanel);
	~GameProtocol();

private:
	void SetChanel(GameChanelData* _chanel);	// ��ͨ�������

private:
	GameChanelData* m_chanel = nullptr;		// ͨ����
	GameRole* m_pRole = nullptr;			// ҵ����
	std::string m_MsgBuf;					// TCP��Ϣ����
};

#endif // !GAME_PROTOCOL_H




