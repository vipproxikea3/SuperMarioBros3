#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.05f;
#define GOOMBA_GRAVITY		 0.0005f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 8

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1

#define DIE_TIME 250

class CGoomba : public CGameObject
{
	float lLimit = -999999;
	float rLimit = 999999;
	DWORD die_start;

	void Revival();
	void SetRevival();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	virtual void SetState(int state);
	void SetActiveArea(float l, float r) {
	    this->lLimit = l;
		this->rLimit = r;
	}
};