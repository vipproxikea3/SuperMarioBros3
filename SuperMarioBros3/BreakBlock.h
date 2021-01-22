#pragma once
#include "GameObject.h"
#include "SwitchBlock.h"

#define BREAKBLOCK_STATE_IDLE	0
#define BREAKBLOCK_STATE_LOCK	100

#define BREAKBLOCK_ANI_IDLE		0
#define	BREAKBLOCK_ANI_LOCK		1

#define BREAKBLOCK_TYPE_DEFAULT	0
#define BREAKBLOCK_TYPE_SWITCH	1
#define BREAKBLOCK_TYPE_LIFE	2
#define BREAKBLOCK_TYPE_COIN	3
#define BREAKBLOCK_TYPE_LEVEL	4

#define BREAKBLOCK_BBOX_WIDTH	16
#define BREAKBLOCK_BBOX_HEIGHT	16

#define BREAK_BLOCK_MAX_COIN	10

#define BREAK_BLOCK_DAMAGE_TIME	50

class CBreakBlock : public CGameObject
{
	int type;
	int countCoin;
	bool damaging;
	DWORD start_damage;
public:
	CBreakBlock(int type) : CGameObject()
	{
		this->SetState(BREAKBLOCK_STATE_IDLE);
		this->type = type;
		countCoin = BREAK_BLOCK_MAX_COIN;
		damaging = false;
		start_damage = NULL;
	}
	~CBreakBlock() {}
	void ShowReward();
	void ShowSwitchBlock();
	void ShowPiece();
	void ShowSuperMushroomLevel();
	void ShowCoin();
	void ShowLevelReward();
	bool IsDamaging() { return damaging; }
	void StartDamage();
	int GetType() { return type; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

