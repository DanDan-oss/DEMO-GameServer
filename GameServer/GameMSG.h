#ifndef GAME_MSG_H
#define GAME_MSG_H

#include <zinx.h>
#include <google/protobuf/message.h>

/* Э����Ϣ */
class GameMSG:public UserData
{
public:
	enum MSG_TYPE {		// �ͻ��˷�������������Ϣ����
		MSG_TYPE_LOGIN_ID_NAME = 1,		// ��¼
		MSG_TYPE_CHAT_CONTENT = 2,
		MSG_TYPE_NEW_POSTION = 3,

		MSG_TYPE_BROADCAST = 200,
		// ����
		MSG_TYPE_LOGOFF_ID_NAME = 201,	
		// ��ȡ��Χ�����Ϣ
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
	MSG_TYPE m_msgType;			// ��Ϣ����
	google::protobuf::Message* m_pMsg = nullptr;	// protobuf������Ϣ�ĸ���
};

class MultGameMSG :public UserData
{
public:
	std::vector<GameMSG*> m_pMsg;
	virtual ~MultGameMSG();
};

#endif // !GAME_MSG_H





