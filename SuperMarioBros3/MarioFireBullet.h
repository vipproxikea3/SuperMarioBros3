#pragma once
#pragma once
#include "GameObject.h"
#include "ParaGoomba.h"
#include "ParaKoopa.h"

#define BULLET_WALKING_SPEED			0.15f
#define BULLET_GRAVITY					0.0005f
#define BULLET_JUMP_DEFLECT_SPEED		0.1f

#define BULLET_STATE_WALKING_RIGHT		100
#define BULLET_STATE_WALKING_LEFT		200

#define MARIO_FIRE_BULLET_ANI_SET_ID	3

#define BULLET_ANI_WALKING_RIGHT		0
#define BULLET_ANI_WALKING_LEFT			1

#define BULLET_BBOX_WIDTH				8
#define BULLET_BBOX_HEIGHT				8

class CMarioFireBullet : public CGameObject
{
public:
	CMarioFireBullet() : CGameObject()
	{
	}
	void BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void ReSet();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

