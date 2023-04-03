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
	this->InitPosin(100, 100, 180, 0);		// 初始化玩家上线位置,先写死
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
	// x轴网格宽度=(x轴结束坐标-x轴起始坐标)/x轴网格数量；y轴的计算方式相同
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
	// TODO: 释放整个地图玩家队列
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
	// 网格编号=(x-x轴起始坐标)/x轴网格宽度 + (y-y轴起始坐标)/y轴宽度 *x轴网格数量
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

	/* 计算玩家所属网格编号*/

	std::list<Player*> players;
	std::list<Player*> pList;
	int gridID = 0;
	int iIndexX = 0;
	int iIndexY = 0;


	gridID = this->GetPlayerGridID(_player);
	iIndexX = gridID % this->m_gripX_count - 1;		// 矩形第一个x行
	iIndexY = gridID / this->m_gripX_count - 1;		// 矩形第一个y行

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j< 3; ++j)
			if (iIndexX + i >= 0 && iIndexY + j >= 0)
			{
				// merge() 会将传入队列的数据剪切进目标队列,源队列数据丢失
				// 所以可以将源队列值赋值给pList, 剪切pList的数据, 不会丢失源队列的数据
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

	// 计算坐标所属网格编号
	gridID = this->GetPlayerGridID(_x, _y);
	iIndexX = gridID % this->m_gripX_count - 1;		// 矩形第一个x行
	iIndexY = gridID / this->m_gripX_count - 1;		// 矩形第一个y行

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (iIndexX + i >= 0 && iIndexY + j >= 0)
			{
				// merge() 会将传入队列的数据剪切进目标队列,源队列数据丢失
				// 所以可以将源队列值赋值给pList, 剪切pList的数据, 不会丢失源队列的数据
				pList = this->m_MapGrip[iIndexX + j + (iIndexY + i) * this->m_gripX_count].m_players;
				players.merge(pList);
			}

	return players;
}

bool WorldAOI::AddPlayer(Player& _player)
{
	// 计算玩家要添加到的网格号
	int gridID = this->GetPlayerGridID(_player);

	// 添加到地图网格中
	this->m_MapGrip[gridID].m_players.push_back(&_player);
	return true;
}

void WorldAOI::RemPlayer(Player& _player)
{
	// 计算要移除玩家的网格号: 
	int gridID = this->GetPlayerGridID(_player);
	this->m_MapGrip[gridID].m_players.remove(&_player);
	
	return;
}

void WorldAOI::PlayerMoveGrip(Player& _player)
{
	
}


