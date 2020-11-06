#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f 

#define MARIO_JUMP_SPEED_Y_WEAK			0.35f
#define MARIO_JUMP_SPEED_Y_STRONG		0.4f
#define MARIO_JUMP_DEFLECT_SPEED		0.2f
#define MARIO_GRAVITY					0.001f
#define MARIO_DIE_DEFLECT_SPEED			0.35f

#define MARIO_STATE_IDLE				0
#define MARIO_STATE_WALKING_RIGHT		100
#define MARIO_STATE_WALKING_LEFT		200
#define MARIO_STATE_JUMP				300
#define MARIO_STATE_DIE					400

#define MARIO_ANI_DIE					0

#define MARIO_ANI_SMALL_IDLE_RIGHT		1
#define MARIO_ANI_SMALL_IDLE_LEFT		2
#define MARIO_ANI_SMALL_WALKING_RIGHT	3
#define MARIO_ANI_SMALL_WALKING_LEFT	4
#define MARIO_ANI_SMALL_JUMP_RIGHT		5
#define MARIO_ANI_SMALL_JUMP_LEFT		6

#define MARIO_ANI_BIG_IDLE_RIGHT		7
#define MARIO_ANI_BIG_IDLE_LEFT			8
#define MARIO_ANI_BIG_WALKING_RIGHT		9
#define MARIO_ANI_BIG_WALKING_LEFT		10
#define MARIO_ANI_BIG_JUMP_RIGHT		11
#define MARIO_ANI_BIG_JUMP_LEFT			12

#define MARIO_ANI_RACCOON_IDLE_RIGHT	13
#define MARIO_ANI_RACCOON_IDLE_LEFT		14
#define MARIO_ANI_RACCOON_WALKING_RIGHT	15
#define MARIO_ANI_RACCOON_WALKING_LEFT	16
#define MARIO_ANI_RACCOON_JUMP_RIGHT	17
#define MARIO_ANI_RACCOON_JUMP_LEFT		18

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACCOON 3

#define MARIO_RACCOON_BBOX_WIDTH  15
#define MARIO_RACCOON_BBOX_HEIGHT 27

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
public:
	CMario() : CGameObject()
	{
		level = MARIO_LEVEL_RACCOON;
		untouchable = 0;
	}
	bool canJump = 0;
	void BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};