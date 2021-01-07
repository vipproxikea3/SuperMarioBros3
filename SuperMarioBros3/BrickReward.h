#pragma once
#include "GameObject.h"

#define BRICKREWARD_JUMP_SPEED_Y	0.075f
#define BRICKREWARD_GRAVITY			0.001f

#define BRICKREWARD_STATE_IDLE		0
#define BRICKREWARD_STATE_LOCK		100
#define BRICKREWARD_STATE_JUMP		200

#define BRICKREWARD_TYPE_COIN		0
#define BRICKREWARD_TYPE_LEVEL		1

#define BRICKREWARD_ANI_IDLE		0
#define BRICKREWARD_ANI_LOCK		1

#define BRICKREWARD_BBOX_WIDTH		16
#define BRICKREWARD_BBOX_HEIGHT		16

class CBrickReward : public CGameObject
{
	int type;

public:
	CBrickReward(int type) : CGameObject()
	{
		this->SetState(BRICKREWARD_STATE_IDLE);
		this->type = type;
	}
	~CBrickReward() {}
	void ShowReward();
	void ShowPoint();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


