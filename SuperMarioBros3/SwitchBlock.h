#pragma once
#include "GameObject.h"
#include "Coin.h"

#define SWITCHBLOCK_STATE_IDLE				0
#define SWITCHBLOCK_STATE_ACTIVED			100

#define SWITCHBLOCK_ANI_IDLE				0
#define	SWITCHBLOCK_ANI_ACTIIVED			1

#define SWITCHBLOCK_BBOX_WIDTH				16
#define SWITCHBLOCK_BBOX_HEIGHT				16
#define SWITCHBLOCK_ACTIVED_BBOX_HEIGHT		7

class CSwitchBlock : public CGameObject
{
public:
	CSwitchBlock() : CGameObject()
	{
		this->SetState(SWITCHBLOCK_STATE_IDLE);
	}
	~CSwitchBlock() {}
	void Switch();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

