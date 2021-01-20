#pragma once
#include "GameObject.h"
#include "Utils.h"

#define LOTTERY_FLY_SPEED		0.075f

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
	bool flying = false;
	bool CanChange;
	DWORD last_change = NULL;
public:
	CLottery() : CGameObject()
	{
		SetSpeed(0, 0);
		flying = false;
		CanChange = true;
		last_change = GetTickCount64();
	}
	~CLottery() {}
	void Fly() {
		flying = true;
		this->SetSpeed(0, -LOTTERY_FLY_SPEED);
		DebugOut(L"[INFO] fly\n");
	}
	void StopChange() { CanChange = false; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
