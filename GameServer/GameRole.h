#ifndef GAME_ROLE_H
#define GAME_ROLE_H

#include <zinx.h>
#include "GameWorld.h"
class GameProtocol;
class GameMSG;

/* 业务层&& 人物角色继承类*/
class GameRole :
	public Player, public Irole 
{
	friend class GameProtocol;
public:
	// 通过 Irole 继承
	virtual bool Init() override;
	virtual UserData* ProcMsg(UserData& _poUserData) override;
	virtual void Fini() override;

	GameRole(GameProtocol* _proto);
	virtual ~GameRole();
	
	void PlayerLogin();	// 玩家上线
	void PlayerLogoff();	// 玩家下线
	void PlayerRadioBroad(std::string& _strData);	// 玩家发送世界聊天消息
	void PlayerMovePos(float _x, float _y, float _z, float _v);	// 移动位置
private:

	void SetProtocol(GameProtocol* _proto);	// 设置协议类

	GameMSG* SendPlayerLogin();				// 玩家上线消息
	GameMSG* SendPlayerLogoff();			// 玩家下线消息
	GameMSG* SendPlayerInfoForSrd();		// 玩家同步周围玩家信息
	GameMSG* SendPlayerInfoForMe();			// 向周围玩家发送的自己信息/玩家新位置消息
	GameMSG* SendConverMessage(std::string& _strData);		// 广播聊天消息


private:
	GameProtocol* m_Proto = nullptr;		// 协议类
};

#endif // GAME_ROLE_H
