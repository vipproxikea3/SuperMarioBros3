#pragma once
#include "GameObject.h"

#define LOTTERY_ANI_MUSHROOM	0
#define LOTTERY_ANI_FLOWER		1
#define LOTTERY_ANI_STAR		2

#define LOTTERY_STATE_MUSHROOM	0
#define LOTTERY_STATE_FLOWER	100
#define LOTTERY_STATE_STAR		200

#define LOTTERY_TIME_CHANGE		200

#define LOTTERY_BBOX_WIDTH		16
#define LOTTERY_BBOX_HEIGHT		16

class CLottery : public CGameObject
{
	DWORD last_change = NULL;
public:
	CLottery() : CGameObject()
	{
		last_change = GetTickCount64();
	}
	~CLottery() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
