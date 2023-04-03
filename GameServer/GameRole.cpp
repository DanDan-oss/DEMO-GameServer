#include <algorithm>
#include <iostream>
#include "GameRole.h"
#include "GameMSG.h"
#include "GameProtocol.h"
#include "msg.pb.h"
#include "GameNetwork.h"



//#define SERVER_DEBUG
static WorldAOI world(0, 400, 0, 400, 20, 20);	// 世界地图AOI类

bool GameRole::Init()
{
	this->PlayerLogin();	// 玩家上线(测试)
	return true;
}


void GameRole::Fini()
{
	this->PlayerLogoff();	// 玩家下线(测试)
}



UserData* GameRole::ProcMsg(UserData& _poUserData)
{
	std::string strData;
	pb::Talk* pTalk = nullptr;
	pb::Position* pPos = nullptr;

	GET_REF2DATA(MultGameMSG, input, _poUserData);
	for (auto single : input.m_pMsg)
	{
		switch (single->GetMsgType())
		{
		case  GameMSG::MSG_TYPE_CHAT_CONTENT:
			pTalk = dynamic_cast<pb::Talk*>(single->GetMessagePoint());
			strData = pTalk->content();
			this->PlayerRadioBroad(strData);
			//this->PlayerRadioBroad((std::string&)(*&(pTalk->content())));
			break;
		case GameMSG::MSG_TYPE_NEW_POSTION:
			pPos = dynamic_cast<pb::Position*>(single->GetMessagePoint());
			this->PlayerMovePos(pPos->posx(), pPos->posz(), pPos->posy(), pPos->posv());
		default:
			break;
		}

		/*测试：打印消息内容*/
		std::cout << "type is" << single->GetMsgType() << std::endl;
		std::cout << single->GetMessagePoint()->Utf8DebugString() << std::endl;

	}

	return nullptr;
}

GameRole::GameRole(GameProtocol* _proto)
	:m_Proto(_proto)
{
}

GameRole::~GameRole()
{


}

void GameRole::SetProtocol(GameProtocol* _proto)
{
	this->m_Proto = _proto;
	return;
}



void GameRole::PlayerLogin()
{

	/* TODO: 玩家上线,创建玩家类
	*   添加玩家到地图AOI网格队列
	*	发送socket消息,获取自己的名称和ID,获取周围玩家的位置,
	*	像周围玩家发送自己的位置*/
	GameMSG* pMsg = nullptr;
	std::string strName = "Test";
	GameRole* pRole = nullptr;

	this->SetWorldMap(&world);
	this->InitPerson(this->m_Proto->m_chanel->GetFd(), strName);
	this->InitPosin(100, 100, 0, 2);
	
	// 向客户端发送自己的名称和id
	pMsg = this->SendPlayerLogin();
	ZinxKernel::Zinx_SendOut(*pMsg, *(this->m_Proto));

	// 将自己添加到地图AOI玩家队列
	world.AddPlayer(*this);

	// 获取玩家地图周围玩家信息
	// ERROR：第二个上线的人 没有同步到之前队列的玩家
	pMsg = this->SendPlayerInfoForSrd();
	ZinxKernel::Zinx_SendOut(*pMsg, *(this->m_Proto));

	// 向周围玩家发送自己的位置
	std::list<Player*> list = this->GetWorldMap()->GetSraPlayers(*this);
	for (Player* pTemp : list)
	{
		pRole = dynamic_cast<GameRole*>(pTemp);
		pMsg = this->SendPlayerInfoForMe();
		ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Proto));
	}
}

void GameRole::PlayerLogoff()
{
	GameMSG* pMsg = nullptr;
	GameRole* pRole = nullptr;
	std::list<Player*> list = this->GetWorldMap()->GetSraPlayers(*this);	// 获取玩家周围玩家

	this->GetWorldMap()->RemPla
yer(*this);	// 将玩家从世界AOI移除
	for (Player* pPlayer : list)
	{	// 循环向玩家之前获取到的附近玩家队列发送玩家下线消息
		pRole = dynamic_cast<GameRole*>(pPlayer);
		pMsg = this->SendPlayerLogoff();
		ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Proto));
	}
}

void GameRole::PlayerRadioBroad(std::string& _strData)
{
	GameMSG* pMsg = nullptr;
	GameRole* pRole = nullptr;
	std::list<Irole*> list = ZinxKernel::Zinx_GetAllRole();

	for (Irole* pTemp : list)
	{
		pRole = dynamic_cast<GameRole*>(pTemp);
		pMsg = this->SendConverMessage(_strData);
		ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Proto));
	}
	return;
}

void GameRole::PlayerMovePos(float _x, float _y, float _z, float _v)
{
	/* TODO: 玩家移动, 离开视野范围的玩家移除,新加入视野玩家添加
	*			同步玩家位置
	*/
	GameMSG* pMsg = nullptr;
	GameRole* pRole = nullptr;
	int oldGridID = this->GetWorldMap()->GetPlayerGridID(*this);
	int newGridID = this->GetWorldMap()->GetPlayerGridID(_x, _y);

	std::list<Player*> oldList;
	std::list<Player*> newList;

	if (oldGridID != newGridID)
	{
		/* 如果玩家移动后,
		AOI网格发生变化则获取当前网格的玩家和移动后的玩家网格,重置玩家网格和位置
		向所有周围网格跑出视野的玩家发送自己下线消息,自己发出抛出视野玩家下线的消息,
		向所有新周围玩家发送自己的位置消息
	 */

		oldList = this->GetWorldMap()->GetSraPlayers(*this);
		// 移出网格队列, 重置玩家坐标,并且添加到新网格队列
		this->GetWorldMap()->RemPlayer(*this);
		this->SetX(_x);
		this->SetY(_y);
		this->SetZ(_z);
		this->SetV(_v);
		this->GetWorldMap()->AddPlayer(*this);

		// 获取新位置的附近玩家信息
		newList = this->GetWorldMap()->GetSraPlayers(*this);

		for (Player* oldPlayer : oldList)
		{
			int status = 0;
			for (Player* newPlayer : newList)
			{
				if (oldPlayer == newPlayer)
					status = 1;
			}
			if (!status)
			{
				pRole = dynamic_cast<GameRole*>(oldPlayer);
				pMsg = this->SendPlayerLogoff();
				ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Proto));
				pMsg = pRole->SendPlayerLogoff();
				ZinxKernel::Zinx_SendOut(*pMsg, *(this->m_Proto));
			}
		}

		for (Player* newPlayer : newList)
		{
			int status = 0;
			for (Player* oldPlayer : oldList)
			{
				if (oldPlayer == newPlayer)
					status = 1;
			}
			if (!status)
			{
				pRole = dynamic_cast<GameRole*>(newPlayer);
				pMsg = this->SendPlayerInfoForMe();
				ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Proto));
				pMsg = pRole->SendPlayerInfoForMe();
				ZinxKernel::Zinx_SendOut(*pMsg, *(this->m_Proto));
			}
		}
		return;
		/*
		std::list<Player*> tmpList;
		// 在新周围玩家和旧周围玩家队列中查找跑出视野的玩家
		for (Player* pTemp : oldList)
		{
			if (newList.end() == find(newList.begin(), newList.end(), pTemp))
				continue;
			tmpList.push_back(pTemp);
		}

		// 向自己发送跑出视野玩家下线的消息,向跑出视野玩家发送自己下线的消息
		for (Player* pTemp : tmpList)
		{
			pRole = dynamic_cast<GameRole*>(pTemp);
			pMsg = pRole->SendPlayerLogoff();
			ZinxKernel::Zinx_SendOut(*pMsg, *(this->m_Proto));
			pMsg = this->SendPlayerLogoff();
			ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Proto));
		}
		*/

		 // 向自己发送新周围玩家信息消息
		//pMsg = this->SendPlayerInfoForSrd();
		//ZinxKernel::Zinx_SendOut(*pMsg, *(this->m_Proto));

		//// 遍历新周围玩家发送自己信息消息
		//for (Player* pTemp : newList)
		//{
		//	pRole = dynamic_cast<GameRole*>(pTemp);
		//	pMsg = this->SendPlayerInfoForMe();
		//	ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Proto));
		//}

		return;
	}
	else {
		this->SetX(_x);
		this->SetY(_y);
		this->SetZ(_z);
		this->SetV(_v);
		// 获取新网格周围玩家队列,向玩家发送新信息消息
		newList = this->GetWorldMap()->GetSraPlayers(*this);
		for (Player* pTemp : newList)
		{
			pRole = dynamic_cast<GameRole*>(pTemp);
			pMsg = this->SendPlayerInfoForMe();
			ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Proto));
			pMsg = pRole->SendPlayerInfoForMe();
			ZinxKernel::Zinx_SendOut(*pMsg, *(this->m_Proto));
		}

		return;
	}
	
	
}

GameMSG* GameRole::SendPlayerLogin()
{
	GameMSG* pMsg = nullptr;
	pb::SyncPid* syncPid = new pb::SyncPid();

	// 发送自己ID和的名称
	syncPid->set_playid(this->GetID());
	syncPid->set_username(this->GetName());
	pMsg = new GameMSG(GameMSG::MSG_TYPE_LOGIN_ID_NAME, syncPid);
	delete syncPid;
	return pMsg;
}

GameMSG* GameRole::SendPlayerLogoff()
{
	GameMSG* pMsg = nullptr;
	pb::SyncPid* spMsg = new pb::SyncPid();

	spMsg->set_playid(this->GetID());
	spMsg->set_username(this->GetName());

	pMsg = new GameMSG(GameMSG::MSG_TYPE_LOGOFF_ID_NAME, spMsg);
	delete spMsg;
	return pMsg;
}

GameMSG* GameRole::SendPlayerInfoForSrd()
 {
	GameMSG* pMsg = nullptr;
	pb::Player* pPlay = nullptr;
	pb::Position* pPos = nullptr;
	GameRole* pRole = nullptr;

	pb::SyncPlayers* syncPlayers = new pb::SyncPlayers();	// 玩家数组消息

	// 周围玩家队列
	WorldAOI* wordA = this->GetWorldMap();
	std::list<Player*> List = wordA->GetSraPlayers(*this);

	// 遍历周围玩家队列,往玩家数组消息赋值
	for (Player* pTemp : List)
	{
		pRole = dynamic_cast<GameRole*>(pTemp);
		pPlay = syncPlayers->add_ps();
		pPlay->set_playid(pRole->GetID());
		pPlay->set_username(pRole->GetName());

		pPos = pPlay->mutable_pos();
		pPos->set_posx(pRole->GetX());
		pPos->set_posy(pRole->GetZ());
		pPos->set_posz(pRole->GetY());
		pPos->set_posv(pRole->GetV());
	}

	// 创建周围玩家信息消息
	pMsg = new GameMSG(GameMSG::MSG_TYPE_SRD_POSTION, syncPlayers);
	delete syncPlayers;
	return pMsg;
}

GameMSG* GameRole::SendPlayerInfoForMe()
{
	GameMSG* pMsg = nullptr;
	pb::Position* pPos = nullptr;
	pb::BroadCast* pBroad = new pb::BroadCast();

	pBroad->set_playid(this->GetID());
	pBroad->set_username(this->GetName());
	pBroad->set_type(2);

	/* 客户端世界的用的是y是用z来表示,y表示高度, 客户端写法已生成,没办法改变*/
	pPos = pBroad->mutable_pos();
	pPos->set_posx(this->GetX());
	pPos->set_posy(this->GetZ());
	pPos->set_posz(this->GetY());
	pPos->set_posv(this->GetV());

	pMsg = new GameMSG(GameMSG::MSG_TYPE_BROADCAST, pBroad);
	delete pBroad;
	return pMsg;
}

GameMSG* GameRole::SendConverMessage(std::string& _strData)
{
	GameMSG* pMsg = nullptr;
	pb::BroadCast* pBroad = new pb::BroadCast();

	pBroad->set_playid(this->GetID());
	pBroad->set_username(this->GetName());
	pBroad->set_type(1);
	pBroad->set_content(_strData);

	pMsg = new GameMSG(GameMSG::MSG_TYPE_BROADCAST, pBroad);
	delete pBroad;
	return pMsg;
}




