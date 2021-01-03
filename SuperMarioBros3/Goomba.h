#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED		0.05f;
#define GOOMBA_GRAVITY				0.0005f
#define GOOMBA_DIE_DEFLECT_SPEED	0.15f

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE_Y	 200
#define GOOMBA_STATE_DIE_X	 300

#define GOOMBA_ANI_WALKING	0
#define GOOMBA_ANI_DIE_Y	1
#define GOOMBA_ANI_DIE_X	2

#define GOOMBA_BBOX_WIDTH		16
#define GOOMBA_BBOX_HEIGHT		16
#define GOOMBA_BBOX_HEIGHT_DIE	8

#define DIE_TIME 700

class CGoomba : public CGameObject
{
	float lLimit = -999999;
	float rLimit = 999999;
	DWORD die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void ReSet();
public:
	CGoomba(float l, float r);
	~CGoomba();
	virtual void SetState(int state);
	void SetActiveArea(float l, float r) {
	    this->lLimit = l;
		this->rLimit = r;
	}
};