#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Backup.h"

#define COIN_JUMP_SPEED_Y		0.2f
#define COIN_GRAVITY			0.001f

#define COIN_STATE_IDLE			0
#define COIN_STATE_JUMP			100

#define COIN_ANI_IDLE			0

#define COIN_BBOX_WIDTH			16
#define COIN_BBOX_HEIGHT		16

#define COIN_TIME_CONVERT		10000

class CCoin : public CGameObject
{
	DWORD start_convert;
	bool CanConvert = false;
public:
	CCoin() : CGameObject()
	{
		this->SetState(COIN_STATE_IDLE);
	}
	~CCoin() {}
	void Jump();
	void SetStartComvert();
	void ConvertToBreakBlock();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
