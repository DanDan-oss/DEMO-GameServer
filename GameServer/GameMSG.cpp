#include "GameMSG.h"
#include "msg.pb.h"

GameMSG::GameMSG(GameMSG::MSG_TYPE _type, google::protobuf::Message* _pMsg)
	: m_msgType(_type)
{
	switch (_type)
	{
	case MSG_TYPE_LOGIN_ID_NAME:
		this->m_pMsg = new pb::SyncPid(dynamic_cast<pb::SyncPid&>(*_pMsg));
		break;
	case MSG_TYPE_CHAT_CONTENT:
		this->m_pMsg = new pb::Talk(dynamic_cast<pb::Talk&>(*_pMsg));
		break;
	case MSG_TYPE_NEW_POSTION:
		this->m_pMsg = new pb::Position(dynamic_cast<pb::Position&>(*_pMsg));
		break;
	case MSG_TYPE_BROADCAST:
		this->m_pMsg = new pb::BroadCast(dynamic_cast<pb::BroadCast&>(*_pMsg));
		break;
	case MSG_TYPE_LOGOFF_ID_NAME:
		this->m_pMsg = new pb::SyncPid(dynamic_cast<pb::SyncPid&>(*_pMsg));
		break;
	case MSG_TYPE_SRD_POSTION:
		this->m_pMsg = new pb::SyncPlayers(dynamic_cast<pb::SyncPlayers&>(*_pMsg));
		break;
	default:
		break;
	}
	
}

GameMSG::GameMSG(MSG_TYPE _type, std::string _stream)
	:m_msgType(_type)
{
	switch (_type)
	{
	case GameMSG::MSG_TYPE_LOGIN_ID_NAME:
		this->m_pMsg = new pb::SyncPid();
		break;
	case GameMSG::MSG_TYPE_CHAT_CONTENT:
		this->m_pMsg = new pb::Talk();
		break;
	case GameMSG::MSG_TYPE_NEW_POSTION:
		this->m_pMsg = new pb::Position();
		//this->m_pMsg = new 
		break;
	case GameMSG::MSG_TYPE_BROADCAST:
		this->m_pMsg = new pb::BroadCast();
		break;
	case GameMSG::MSG_TYPE_LOGOFF_ID_NAME:
		this->m_pMsg = new pb::SyncPid();
		break;
	case GameMSG::MSG_TYPE_SRD_POSTION:
		this->m_pMsg = new pb::SyncPlayers();
		break;
	default:
		break;
	}
	this->m_pMsg->ParseFromString(_stream);
}

GameMSG::~GameMSG()
{
	if (this->m_pMsg)
		delete this->m_pMsg;
}

std::string GameMSG::Serialize()
{
	std::string str;
	this->m_pMsg->SerializeToString(&str);
	return str;

}

google::protobuf::Message* GameMSG::GetMessagePoint()
{
	return this->m_pMsg;
}

GameMSG::MSG_TYPE GameMSG::GetMsgType()
{
	return this->m_msgType;
}

MultGameMSG::~MultGameMSG()
{
	// Î´ÊÍ·ÅÐÅºÅ¼¯
	for (GameMSG* pMsg : this->m_pMsg)
		delete pMsg;
}
