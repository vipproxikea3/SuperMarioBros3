#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.05f;
#define GOOMBA_GRAVITY		 0.002f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 8

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1

class CGoomba : public CGameObject
{
	float lLimit, rLimit;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	virtual void SetState(int state);
	void SetActiveArea(float l = -9999, float r = 9999) {
	    this->lLimit = l;
		this->rLimit = r;
	}
};