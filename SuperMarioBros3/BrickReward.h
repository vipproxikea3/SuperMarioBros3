#pragma once
#include "GameObject.h"

#define BRICKREWARD_JUMP_SPEED_Y	0.2f
#define BRICKREWARD_GRAVITY			0.001f

#define BRICKREWARD_STATE_IDLE		0
#define BRICKREWARD_STATE_ACTIVED	100
#define BRICKREWARD_STATE_JUMP		200

#define BRICKREWARD_ANI_IDLE	0
#define BRICKREWARD_ANI_ACTIVED 1

#define BRICKREWARD_BBOX_WIDTH  16
#define BRICKREWARD_BBOX_HEIGHT 16

class CBrickReward : public CGameObject
{
	CGameObject* reward;

public:
	CBrickReward() : CGameObject()
	{
		this->SetState(BRICKREWARD_STATE_IDLE);
	}
	void setReward(CGameObject* GObj);
	void showReward();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


