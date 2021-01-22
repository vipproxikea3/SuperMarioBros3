#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Utils.h"
#include "Boomerang.h"

#define BOOMERANGBROTHER_WALKING_SPEED			0.05f
#define BOOMERANGBROTHER_GRAVITY				0.001f

#define BOOMERANGBROTHER_STATE_READY_LEFT		0
#define BOOMERANGBROTHER_STATE_ATTACK_LEFT		100
#define BOOMERANGBROTHER_STATE_READY_RIGHT		300
#define BOOMERANGBROTHER_STATE_ATTACK_RIGHT		400


#define BOOMERANGBROTHER_ANI_ATTACK_LEFT		0
#define BOOMERANGBROTHER_ANI_READY_LEFT			1
#define BOOMERANGBROTHER_ANI_ATTACK_RIGHT		2
#define BOOMERANGBROTHER_ANI_READY_RIGHT		3
#define BOOMERANGBROTHER_ANI_BOOMERANG_LEFT		4
#define BOOMERANGBROTHER_ANI_BOOMERANG_RIGHT	5

#define BOOMERANGBROTHER_BBOX_WIDTH				16
#define BOOMERANGBROTHER_BBOX_HEIGHT			24

#define BOOMERANGBROTHER_WALKING_ZONE			16

#define BOOMERANGBROTHER_TIME_SHOT				2000

class CBoomerangBrother : public CGameObject
{
	DWORD last_shot = NULL;
	bool shoted;
public:
	CBoomerangBrother() : CGameObject()
	{
		this->SetState(BOOMERANGBROTHER_STATE_READY_RIGHT);
		vx = BOOMERANGBROTHER_WALKING_SPEED;
	}
	~CBoomerangBrother() {}
	void SetSide();
	void Shot();
	void ReSet();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
