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

#define BREAKBLOCK_BBOX_WIDTH	16
#define BREAKBLOCK_BBOX_HEIGHT	16

class CBreakBlock : public CGameObject
{
	int type;
public:
	CBreakBlock(int type) : CGameObject()
	{
		this->SetState(BREAKBLOCK_STATE_IDLE);
		this->type = type;
	}
	~CBreakBlock() {}
	void ShowReward();
	void ShowSwitchBlock();
	void ShowSuperMushroomLevel();
	int GetType() { return type; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

