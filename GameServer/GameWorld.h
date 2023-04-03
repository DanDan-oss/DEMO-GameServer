#ifndef GAME_WORLD_H
#define GAME_WORLD_H	// 游戏世界类
#include <list>
#include <vector>
#include <iostream>

class WorldAOI;
class GameRole;

// 万物基类
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

// 人物基类
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
	float m_curV = 0;		// 视角,范围从0-360
	float m_curZ = 0;
	int m_ID = 0;
	std::string m_strName;

};

// 玩家类
class Player : public Person	
{
public:
	Player(const WorldAOI* _world = nullptr);
	~Player();
};

struct Map_Grip		// 地图网格 格子类
{
	std::list<Player*> m_players;		// 玩家
	//std::list<Monster*> m_monsters;  // 怪物
	//std::list<Goods*> m_goods;		// 物品
	//std::list<NPC*> m_npc;			// npc
};

class WorldAOI
{
public:
	/* 通过构造函数指定地图矩形大小和分割粒度 */
	WorldAOI(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count=20, int _y_count=20);
	virtual ~WorldAOI();

	int GetPlayerGridID(Player& _player);
	int GetPlayerGridID(int _x, int _y);
	int GetGripXCount();
	int GetGripYCount();

	/* 获取附近玩家队列 */
	std::list<Player*> GetSraPlayers(Player& _player);

	/* 获取附近玩家队列 */
	std::list<Player*> GetSraPlayers(int _x, int _y);

	/* 添加玩家到地图网格的玩家队列 */
	bool AddPlayer(Player& _player);

	/* 将玩家从地图网格的玩家队列移除 */
	void RemPlayer(Player& _player);	

	// 移动玩家网格
	void PlayerMoveGrip(Player& _player);



private:
	WorldAOI() {}

public:

private:
	int m_coorX_begin = 0;	// 地图坐标X起始位置
	int m_coorX_end = 0;	// 地图坐标X结束位置
	int m_gripX_count = 0;	// 地图网格 每行分多少格子
	int m_gripX_width = 0;	// 地图网格 每个格子宽度

	int m_coorY_begin = 0;	// 地图坐标Y起始位置
	int m_coorY_end = 0;	// 地图坐标Y结束位置
	int m_gripY_count = 0;	// 地图网格 每列分多少格子
	int m_gripY_width = 0;	// 地图网格 每个格子高度

	std::vector< Map_Grip > m_MapGrip;	// 每个格子
};

#endif // GAME_WORLD_H
