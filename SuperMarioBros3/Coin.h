#pragma once
#include "GameObject.h"

#define COIN_JUMP_SPEED_Y		0.4f
#define COIN_GRAVITY			0.002f

#define COIN_STATE_IDLE			0
#define COIN_STATE_JUMP			100

#define COIN_ANI_IDLE			0

#define COIN_BBOX_WIDTH			10
#define COIN_BBOX_HEIGHT		16

class CCoin : public CGameObject
{
public:
	CCoin() : CGameObject()
	{
		this->SetState(COIN_STATE_IDLE);
	}
	~CCoin() {}
	void Jump();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
