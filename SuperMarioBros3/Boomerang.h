#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Utils.h"
#include "BoomerangBrother.h"

#define BOOMERANG_WALKING_SPEED						0.1f
#define BOOMERANG_FLY_SPEED_PHARSE_1				0.05f
#define BOOMERANG_FLY_SPEED_PHARSE_2				0.1f
#define BOOMERANG_FLY_SPEED_PHARSE_3				0.0f

#define BOOMERANG_STATE_WALKING_LEFT_PHARSE_1		0
#define BOOMERANG_STATE_WALKING_LEFT_PHARSE_2		100
#define BOOMERANG_STATE_WALKING_LEFT_PHARSE_3		200
#define BOOMERANG_STATE_WALKING_RIGHT_PHARSE_1		300
#define BOOMERANG_STATE_WALKING_RIGHT_PHARSE_2		400
#define BOOMERANG_STATE_WALKING_RIGHT_PHARSE_3		500

#define BOOMERANG_ANI_WALKING_LEFT					0
#define BOOMERANG_ANI_WALKING_RIGHT					1

#define BOOMERANG_BBOX_WIDTH						16
#define BOOMERANG_BBOX_HEIGHT						16

#define BOOMERANG_FLY_ZONE							64

class CBoomerang : public CGameObject
{
public:
	CBoomerang() : CGameObject()
	{
		this->SetState(BOOMERANG_STATE_WALKING_RIGHT_PHARSE_1);
	}
	~CBoomerang() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
