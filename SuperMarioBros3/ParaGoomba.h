#pragma once
#include "GameObject.h"

#define PARAGOOMBA_WALKING_SPEED			0.05f;
#define PARAGOOMBA_JUMP_SPEED				0.2f;
#define PARAGOOMBA_GRAVITY					0.0005f
#define PARAGOOMBA_DIE_DEFLECT_SPEED		0.15f

#define PARAGOOMBA_STATE_WALKING			100
#define PARAGOOMBA_STATE_DIE_Y				200
#define PARAGOOMBA_STATE_DIE_X				300

#define PARAGOOMBA_ANI_WALKING				0
#define PARAGOOMBA_ANI_DIE_X				1
#define PARAGOOMBA_ANI_DIE_Y				2
#define PARAGOOMBA_WING_RIGHT_ANI_WALKING	3
#define PARAGOOMBA_WING_RIGHT_ANI_JUMP		4
#define PARAGOOMBA_WING_LEFT_ANI_WALKING	5
#define PARAGOOMBA_WING_LEFT_ANI_JUMP		6

#define PARAGOOMBA_LEVEL_WING				1
#define PARAGOOMBA_LEVEL_GOOMBA				2

#define PARAGOOMBA_BBOX_WIDTH				16
#define PARAGOOMBA_BBOX_HEIGHT				16
#define PARAGOOMBA_BBOX_HEIGHT_DIE			8

#define PARAGOOMBA_DIE_TIME					700
#define PARAGOOMBA_JUMP_COOLDOWN			2000

class CParaGoomba : public CGameObject
{
	float lLimit = -999999;
	float rLimit = 999999;
	int level;
	DWORD die_start;
	DWORD last_jump;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void ReSet();
public:
	CParaGoomba(float l, float r);
	~CParaGoomba();
	virtual void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	void SetActiveArea(float l, float r) {
		this->lLimit = l;
		this->rLimit = r;
	}
};