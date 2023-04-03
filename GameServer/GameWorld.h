#ifndef GAME_WORLD_H
#define GAME_WORLD_H	// ��Ϸ������
#include <list>
#include <vector>
#include <iostream>

class WorldAOI;
class GameRole;

// �������
class ObjectBase		
{
public:
	ObjectBase(const WorldAOI* _world=nullptr);
	virtual float GetX() =0;
	virtual float GetY() =0;
	virtual float GetZ() = 0;
	virtual float GetV() = 0;
	virtual std::string GetName()=0;
	virtual int GetID() = 0;
	WorldAOI* GetWorldMap();
	void SetWorldMap(WorldAOI* _world);
private:
	const WorldAOI* m_world;
};

// �������
class Person : public ObjectBase
{
public:
	Person();
	Person(const WorldAOI* _world);
	virtual ~Person();

	void InitPerson(int _ID, std::string& _playerName);
	void InitPosin(float _coorX, float _coorY, float _coorV, float _coorZ);

	virtual float GetX();
	virtual float GetY();
	virtual float GetZ();
	virtual float GetV();
	virtual std::string GetName();
	virtual int GetID();

	void SetX(float _x);
	void SetY(float _y);
	void SetZ(float _z);
	void SetV(float _v);

private:
	float m_curX = 0;
	float m_curY = 0;	
	float m_curV = 0;		// �ӽ�,��Χ��0-360
	float m_curZ = 0;
	int m_ID = 0;
	std::string m_strName;

};

// �����
class Player : public Person	
{
public:
	Player(const WorldAOI* _world = nullptr);
	~Player();
};

struct Map_Grip		// ��ͼ���� ������
{
	std::list<Player*> m_players;		// ���
	//std::list<Monster*> m_monsters;  // ����
	//std::list<Goods*> m_goods;		// ��Ʒ
	//std::list<NPC*> m_npc;			// npc
};

class WorldAOI
{
public:
	/* ͨ�����캯��ָ����ͼ���δ�С�ͷָ����� */
	WorldAOI(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count=20, int _y_count=20);
	virtual ~WorldAOI();

	int GetPlayerGridID(Player& _player);
	int GetPlayerGridID(int _x, int _y);
	int GetGripXCount();
	int GetGripYCount();

	/* ��ȡ������Ҷ��� */
	std::list<Player*> GetSraPlayers(Player& _player);

	/* ��ȡ������Ҷ��� */
	std::list<Player*> GetSraPlayers(int _x, int _y);

	/* �����ҵ���ͼ�������Ҷ��� */
	bool AddPlayer(Player& _player);

	/* ����Ҵӵ�ͼ�������Ҷ����Ƴ� */
	void RemPlayer(Player& _player);	

	// �ƶ��������
	void PlayerMoveGrip(Player& _player);



private:
	WorldAOI() {}

public:

private:
	int m_coorX_begin = 0;	// ��ͼ����X��ʼλ��
	int m_coorX_end = 0;	// ��ͼ����X����λ��
	int m_gripX_count = 0;	// ��ͼ���� ÿ�зֶ��ٸ���
	int m_gripX_width = 0;	// ��ͼ���� ÿ�����ӿ��

	int m_coorY_begin = 0;	// ��ͼ����Y��ʼλ��
	int m_coorY_end = 0;	// ��ͼ����Y����λ��
	int m_gripY_count = 0;	// ��ͼ���� ÿ�зֶ��ٸ���
	int m_gripY_width = 0;	// ��ͼ���� ÿ�����Ӹ߶�

	std::vector< Map_Grip > m_MapGrip;	// ÿ������
};

#endif // GAME_WORLD_H
