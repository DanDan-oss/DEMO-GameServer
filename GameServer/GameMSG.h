#ifndef GAME_MSG_H
#define GAME_MSG_H

#include <zinx.h>
#include <google/protobuf/message.h>

/* 协议消息 */
class GameMSG:public UserData
{
public:
	enum MSG_TYPE {		// 客户端服务器交互的消息类型
		MSG_TYPE_LOGIN_ID_NAME = 1,		// 登录
		MSG_TYPE_CHAT_CONTENT = 2,
		MSG_TYPE_NEW_POSTION = 3,

		MSG_TYPE_BROADCAST = 200,
		// 下线
		MSG_TYPE_LOGOFF_ID_NAME = 201,	
		// 获取周围玩家信息
		MSG_TYPE_SRD_POSTION = 202		
	};

public:
	GameMSG(GameMSG::MSG_TYPE _type, google::protobuf::Message* _pMsg);
	GameMSG(GameMSG::MSG_TYPE _type, std::string _stream);
	virtual ~GameMSG();

	std::string Serialize();
	google::protobuf::Message* GetMessagePoint();
	GameMSG::MSG_TYPE GetMsgType();

private:
	MSG_TYPE m_msgType;			// 消息类型
	google::protobuf::Message* m_pMsg = nullptr;	// protobuf所有消息的父类
};

class MultGameMSG :public UserData
{
public:
	std::vector<GameMSG*> m_pMsg;
	virtual ~MultGameMSG();
};

#endif // !GAME_MSG_H





