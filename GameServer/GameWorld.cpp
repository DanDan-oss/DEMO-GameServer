#include "GameWorld.h"
#include <iostream>
#include "main.h"

ObjectBase::ObjectBase(const WorldAOI* _world)
	: m_world(_world)
{
}

WorldAOI* ObjectBase::GetWorldMap()
{
	return (WorldAOI*)this->m_world;
}

void ObjectBase::SetWorldMap(WorldAOI* _world)
{
	this->m_world = _world;
	return;
}

// class  Person=============================================
Person::Person()
{
}

Person::Person(const WorldAOI* _world = nullptr)
	: ObjectBase(_world)
{
}

Person::~Person()
{
}

float Person::GetX()
{
	return this->m_curX;
}

float Person::GetY()
{
	return this->m_curY;
}

float Person::GetZ()
{
	return this->m_curZ;
}

float Person::GetV()
{
	return this->m_curV;
}

std::string Person::GetName()
{
	return this->m_strName;
}

int Person::GetID()
{
	return this->m_ID;
}

void Person::SetX(float _x)
{
	this->m_curX = _x;
}
void Person::SetY(float _y)
{
	this->m_curY = _y;
}
void Person::SetZ(float _z)
{
	this->m_curZ = _z;
}
void Person::SetV(float _v)
{
	this->m_curV = _v;
}

void Person::InitPerson(int _ID, std::string& _playerName)
{
	this->m_ID = _ID;
	this->m_strName = _playerName;
	this->InitPosin(100, 100, 180, 0);		// ��ʼ���������λ��,��д��
	return;
}

void Person::InitPosin(float _coorX, float _coorY, float _coorV, float _coorZ)
{
	this->m_curX = _coorX;
	this->m_curY = _coorY;
	this->m_curV = _coorV;
	this->m_curZ = _coorZ;
	return;
}

// class  Player =============================================
Player::Player(const WorldAOI* _world)
	: Person(_world)
{
}

Player::~Player()
{
	DbgPrintA("gamelog", "%s : delete...", this->GetName());
	//std::cout << this->GetName() << ": delete..." << std::endl;
}

// class  Player =============================================
WorldAOI::WorldAOI(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count)
	:m_coorX_begin(_x_begin), m_coorX_end(_x_end), m_gripX_count(_x_count),
	m_coorY_begin(_y_begin), m_coorY_end(_y_end), m_gripY_count(_y_count)
{
	// x��������=(x���������-x����ʼ����)/x������������y��ļ��㷽ʽ��ͬ
	this->m_gripX_width = (this->m_coorX_end - this->m_coorX_begin) / this->m_gripX_count;
	this->m_gripY_width = (this->m_coorY_end - this->m_coorY_begin) / this->m_gripY_count;

	for (int i = 0; i < _x_count * _y_count; ++i)
	{
		Map_Grip tmp;
		m_MapGrip.push_back(tmp);
	}
	return;
}

WorldAOI::~WorldAOI()
{
	// TODO: �ͷ�������ͼ��Ҷ���
	for(int i=0; i< this->m_gripY_count; ++i)
		for (int j = 0; j < this->m_gripX_count; ++j)
		{
			std::list<Player*>* pList = &(this->m_MapGrip[i * this->m_gripX_count + j].m_players);
			for (Player* temp : *pList)
				delete temp;
		}
	return;
}


int WorldAOI::GetPlayerGridID(Player& _player)
{
	// ������=(x-x����ʼ����)/x�������� + (y-y����ʼ����)/y���� *x����������
	return ((int)_player.GetX() - this->m_coorX_begin) / this->m_gripX_width +
		((int)_player.GetY() - this->m_coorY_begin) / this->m_gripY_width * this->m_gripX_count;
}

int WorldAOI::GetPlayerGridID(int _x, int _y)
{
	return (_x - this->m_coorX_begin) / this->m_gripX_width +
		(_y - this->m_coorY_begin) / this->m_gripY_width * this->m_gripX_count;
}

int WorldAOI::GetGripXCount()
{
	return this->m_gripX_count;
}

int WorldAOI::GetGripYCount()
{
	return this->m_gripY_count;
}

std::list<Player*> WorldAOI::GetSraPlayers(Player& _player)
{

	/* �����������������*/

	std::list<Player*> players;
	std::list<Player*> pList;
	int gridID = 0;
	int iIndexX = 0;
	int iIndexY = 0;


	gridID = this->GetPlayerGridID(_player);
	iIndexX = gridID % this->m_gripX_count - 1;		// ���ε�һ��x��
	iIndexY = gridID / this->m_gripX_count - 1;		// ���ε�һ��y��

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j< 3; ++j)
			if (iIndexX + i >= 0 && iIndexY + j >= 0)
			{
				// merge() �Ὣ������е����ݼ��н�Ŀ�����,Դ�������ݶ�ʧ
				// ���Կ��Խ�Դ����ֵ��ֵ��pList, ����pList������, ���ᶪʧԴ���е�����
				pList = this->m_MapGrip[iIndexX + j + (iIndexY + i) * this->m_gripX_count].m_players;
				players.merge(pList);
			}

	return players;
}

std::list<Player*> WorldAOI::GetSraPlayers(int _x, int _y)
{
	std::list<Player*> players;
	std::list<Player*> pList;
	int gridID = 0;
	int iIndexX = 0;
	int iIndexY = 0;

	// ������������������
	gridID = this->GetPlayerGridID(_x, _y);
	iIndexX = gridID % this->m_gripX_count - 1;		// ���ε�һ��x��
	iIndexY = gridID / this->m_gripX_count - 1;		// ���ε�һ��y��

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (iIndexX + i >= 0 && iIndexY + j >= 0)
			{
				// merge() �Ὣ������е����ݼ��н�Ŀ�����,Դ�������ݶ�ʧ
				// ���Կ��Խ�Դ����ֵ��ֵ��pList, ����pList������, ���ᶪʧԴ���е�����
				pList = this->m_MapGrip[iIndexX + j + (iIndexY + i) * this->m_gripX_count].m_players;
				players.merge(pList);
			}

	return players;
}

bool WorldAOI::AddPlayer(Player& _player)
{
	// �������Ҫ��ӵ��������
	int gridID = this->GetPlayerGridID(_player);

	// ��ӵ���ͼ������
	this->m_MapGrip[gridID].m_players.push_back(&_player);
	return true;
}

void WorldAOI::RemPlayer(Player& _player)
{
	// ����Ҫ�Ƴ���ҵ������: 
	int gridID = this->GetPlayerGridID(_player);
	this->m_MapGrip[gridID].m_players.remove(&_player);
	
	return;
}

void WorldAOI::PlayerMoveGrip(Player& _player)
{
	
}


