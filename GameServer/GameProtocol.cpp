#include "GameProtocol.h"
#include "GameMSG.h"
#include "GameNetwork.h"
#include "GameRole.h"
#include "msg.pb.h"

// #define  SERVER_DEBUG
GameProtocol::GameProtocol()
{
	this->m_pRole = new GameRole(this);
	ZinxKernel::Zinx_Add_Role(*(this->m_pRole));
}

GameProtocol::GameProtocol(GameChanelData* _chanel)
	: m_chanel(_chanel)
{
	this->m_pRole = new GameRole(this);
	ZinxKernel::Zinx_Add_Role(*(this->m_pRole));
}

GameProtocol::~GameProtocol()
{
	if (this->m_pRole)
	{
		ZinxKernel::Zinx_Del_Role(*(this->m_pRole));
		delete this->m_pRole;
	}

}


UserData* GameProtocol::raw2request(std::string _szInput)
{	
	int iMsgType = 0;
	int iMsgSize = 0;
	GameMSG* pMsg = nullptr;;
	MultGameMSG* pMsgs = new MultGameMSG();
	// 判断当前当前socket连接的玩家是否已经登录


	// TODO: 将TCP消息转换成游戏逻辑消息 && TCP粘包处理
	this->m_MsgBuf.append(_szInput);
	while (this->m_MsgBuf.size() > 8)
	{
		iMsgSize = 0;
		iMsgSize |= this->m_MsgBuf[0] << 0;
		iMsgSize |= this->m_MsgBuf[1] << 0x8;
		iMsgSize |= this->m_MsgBuf[2] << 0x16;
		iMsgSize |= this->m_MsgBuf[3] << 0x24;

		iMsgType = 0;
		iMsgType |= this->m_MsgBuf[4] << 0;
		iMsgType |= this->m_MsgBuf[5] << 0x8;
		iMsgType |= this->m_MsgBuf[6] << 0x16;
		iMsgType |= this->m_MsgBuf[7] << 0x24;
		// 0a0000000100000008641206686168616861
		// 当前拥有的数据小于应接收到的数据包数据
		int leng = this->m_MsgBuf.size() - 8;
		if (leng < iMsgSize)
			break;
		pMsg = new GameMSG((GameMSG::MSG_TYPE)iMsgType, this->m_MsgBuf.substr(8, iMsgSize));
		pMsgs->m_pMsg.push_back(pMsg);
		this->m_MsgBuf.erase(0, 8 + iMsgSize);
		continue;
	}

 	return pMsgs;
}

std::string* GameProtocol::response2raw(UserData& _oUserData)
{	// TODO: 将游戏逻辑消息转换成TCP消息
	int iLength = 0;
	int iId = 0;
	std::string* pStrMsg = nullptr;
	std::string strMsgContent;


	GET_REF2DATA(GameMSG, oOutput, _oUserData);
	strMsgContent = oOutput.Serialize();
	iLength = strMsgContent.size();
	iId = oOutput.GetMsgType();

	pStrMsg = new std::string();
	pStrMsg->push_back((iLength >> 0) & 0xFF);
	pStrMsg->push_back((iLength >> 8) & 0xFF);
	pStrMsg->push_back((iLength >> 16) & 0xFF);
	pStrMsg->push_back((iLength >> 24) & 0xFF);
	pStrMsg->push_back((iId >> 0) & 0xFF);
	pStrMsg->push_back((iId >> 8) & 0xFF);
	pStrMsg->push_back((iId >> 16) & 0xFF);
	pStrMsg->push_back((iId >> 24) & 0xFF);
	pStrMsg->append(strMsgContent);

	return pStrMsg;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{

	return this->m_pRole;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
	return this->m_chanel;
}


void GameProtocol::SetChanel(GameChanelData* _chanel)
{
	this->m_chanel = _chanel;
}
