#ifndef GAME_PROTOCOL_H
#define GAME_PROTOCOL_H

#include <zinx.h>
class GameChanelData;
class GameRole;

/* 协议层
由于是通道层对象主动创建或者绑定协议层对象(数据由下层往上层传),
所有通道层和协议层对象之间的关系由通道层维护.
业务层由协议层维护,协议层释放时将业务层释放 */
class GameProtocol : public Iprotocol
{
	friend class GameChanelData;
	friend class GameRole;

public:
	// 通过 Iprotocol 继承
	virtual UserData* raw2request(std::string _szInput) override;
	virtual std::string* response2raw(UserData& _oUserData) override;
	virtual Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
	virtual Ichannel* GetMsgSender(BytesMsg& _oBytes) override;

	GameProtocol();
	GameProtocol(GameChanelData* _chanel);
	~GameProtocol();

private:
	void SetChanel(GameChanelData* _chanel);	// 绑定通道层对象

private:
	GameChanelData* m_chanel = nullptr;		// 通道类
	GameRole* m_pRole = nullptr;			// 业务类
	std::string m_MsgBuf;					// TCP消息缓存
};

#endif // !GAME_PROTOCOL_H




