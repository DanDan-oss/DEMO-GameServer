#ifndef GAME_ROLE_H
#define GAME_ROLE_H

#include <zinx.h>
#include "GameWorld.h"
class GameProtocol;
class GameMSG;

/* ҵ���&& �����ɫ�̳���*/
class GameRole :
	public Player, public Irole 
{
	friend class GameProtocol;
public:
	// ͨ�� Irole �̳�
	virtual bool Init() override;
	virtual UserData* ProcMsg(UserData& _poUserData) override;
	virtual void Fini() override;

	GameRole(GameProtocol* _proto);
	virtual ~GameRole();
	
	void PlayerLogin();	// �������
	void PlayerLogoff();	// �������
	void PlayerRadioBroad(std::string& _strData);	// ��ҷ�������������Ϣ
	void PlayerMovePos(float _x, float _y, float _z, float _v);	// �ƶ�λ��
private:

	void SetProtocol(GameProtocol* _proto);	// ����Э����

	GameMSG* SendPlayerLogin();				// ���������Ϣ
	GameMSG* SendPlayerLogoff();			// ���������Ϣ
	GameMSG* SendPlayerInfoForSrd();		// ���ͬ����Χ�����Ϣ
	GameMSG* SendPlayerInfoForMe();			// ����Χ��ҷ��͵��Լ���Ϣ/�����λ����Ϣ
	GameMSG* SendConverMessage(std::string& _strData);		// �㲥������Ϣ


private:
	GameProtocol* m_Proto = nullptr;		// Э����
};

#endif // GAME_ROLE_H
