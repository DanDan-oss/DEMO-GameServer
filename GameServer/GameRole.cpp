#include <algorithm>
#include <iostream>
#include "GameRole.h"
#include "GameMSG.h"
#include "GameProtocol.h"
#include "msg.pb.h"
#include "GameNetwork.h"



//#define SERVER_DEBUG
static WorldAOI world(0, 400, 0, 400, 20, 20);	// �����ͼAOI��

bool GameRole::Init()
{
	this->PlayerLogin();	// �������(����)
	return true;
}


void GameRole::Fini()
{
	this->PlayerLogoff();	// �������(����)
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

		/*���ԣ���ӡ��Ϣ����*/
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

	/* TODO: �������,���������
	*   �����ҵ���ͼAOI�������
	*	����socket��Ϣ,��ȡ�Լ������ƺ�ID,��ȡ��Χ��ҵ�λ��,
	*	����Χ��ҷ����Լ���λ��*/
	GameMSG* pMsg = nullptr;
	std::string strName = "Test";
	GameRole* pRole = nullptr;

	this->SetWorldMap(&world);
	this->InitPerson(this->m_Proto->m_chanel->GetFd(), strName);
	this->InitPosin(100, 100, 0, 2);
	
	// ��ͻ��˷����Լ������ƺ�id
	pMsg = this->SendPlayerLogin();
	ZinxKernel::Zinx_SendOut(*pMsg, *(this->m_Proto));

	// ���Լ���ӵ���ͼAOI��Ҷ���
	world.AddPlayer(*this);

	// ��ȡ��ҵ�ͼ��Χ�����Ϣ
	// ERROR���ڶ������ߵ��� û��ͬ����֮ǰ���е����
	pMsg = this->SendPlayerInfoForSrd();
	ZinxKernel::Zinx_SendOut(*pMsg, *(this->m_Proto));

	// ����Χ��ҷ����Լ���λ��
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
	std::list<Player*> list = this->GetWorldMap()->GetSraPlayers(*this);	// ��ȡ�����Χ���

	this->GetWorldMap()->RemPla
yer(*this);	// ����Ҵ�����AOI�Ƴ�
	for (Player* pPlayer : list)
	{	// ѭ�������֮ǰ��ȡ���ĸ�����Ҷ��з������������Ϣ
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
	/* TODO: ����ƶ�, �뿪��Ұ��Χ������Ƴ�,�¼�����Ұ������
	*			ͬ�����λ��
	*/
	GameMSG* pMsg = nullptr;
	GameRole* pRole = nullptr;
	int oldGridID = this->GetWorldMap()->GetPlayerGridID(*this);
	int newGridID = this->GetWorldMap()->GetPlayerGridID(_x, _y);

	std::list<Player*> oldList;
	std::list<Player*> newList;

	if (oldGridID != newGridID)
	{
		/* �������ƶ���,
		AOI�������仯���ȡ��ǰ�������Һ��ƶ�����������,������������λ��
		��������Χ�����ܳ���Ұ����ҷ����Լ�������Ϣ,�Լ������׳���Ұ������ߵ���Ϣ,
		����������Χ��ҷ����Լ���λ����Ϣ
	 */

		oldList = this->GetWorldMap()->GetSraPlayers(*this);
		// �Ƴ��������, �����������,������ӵ����������
		this->GetWorldMap()->RemPlayer(*this);
		this->SetX(_x);
		this->SetY(_y);
		this->SetZ(_z);
		this->SetV(_v);
		this->GetWorldMap()->AddPlayer(*this);

		// ��ȡ��λ�õĸ��������Ϣ
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
		// ������Χ��Һ;���Χ��Ҷ����в����ܳ���Ұ�����
		for (Player* pTemp : oldList)
		{
			if (newList.end() == find(newList.begin(), newList.end(), pTemp))
				continue;
			tmpList.push_back(pTemp);
		}

		// ���Լ������ܳ���Ұ������ߵ���Ϣ,���ܳ���Ұ��ҷ����Լ����ߵ���Ϣ
		for (Player* pTemp : tmpList)
		{
			pRole = dynamic_cast<GameRole*>(pTemp);
			pMsg = pRole->SendPlayerLogoff();
			ZinxKernel::Zinx_SendOut(*pMsg, *(this->m_Proto));
			pMsg = this->SendPlayerLogoff();
			ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Proto));
		}
		*/

		 // ���Լ���������Χ�����Ϣ��Ϣ
		//pMsg = this->SendPlayerInfoForSrd();
		//ZinxKernel::Zinx_SendOut(*pMsg, *(this->m_Proto));

		//// ��������Χ��ҷ����Լ���Ϣ��Ϣ
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
		// ��ȡ��������Χ��Ҷ���,����ҷ�������Ϣ��Ϣ
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

	// �����Լ�ID�͵�����
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

	pb::SyncPlayers* syncPlayers = new pb::SyncPlayers();	// ���������Ϣ

	// ��Χ��Ҷ���
	WorldAOI* wordA = this->GetWorldMap();
	std::list<Player*> List = wordA->GetSraPlayers(*this);

	// ������Χ��Ҷ���,�����������Ϣ��ֵ
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

	// ������Χ�����Ϣ��Ϣ
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

	/* �ͻ���������õ���y����z����ʾ,y��ʾ�߶�, �ͻ���д��������,û�취�ı�*/
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




