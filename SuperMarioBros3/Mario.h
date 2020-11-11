#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED_BASE		0.05f
#define MARIO_WALKING_SPEED				0.1f
#define MARIO_RUN_SPEED					0.15f
#define MARIO_ACCELERATION				0.0001f
#define MARIO_WALKING_FRICTION			0.00025f

#define MARIO_JUMP_SPEED_Y_WEAK			0.35f
#define MARIO_JUMP_SPEED_Y_STRONG		0.4f
#define MARIO_JUMP_DEFLECT_SPEED		0.2f
#define MARIO_GRAVITY					0.001f
#define MARIO_DIE_DEFLECT_SPEED			0.35f

#define MARIO_STATE_IDLE				0
#define MARIO_STATE_WALKING_RIGHT		100
#define MARIO_STATE_WALKING_LEFT		200
#define MARIO_STATE_RUN_RIGHT			300
#define MARIO_STATE_RUN_LEFT			400
#define MARIO_STATE_JUMP				500
#define MARIO_STATE_DIE					600

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
#define MARIO_ANI_BIG_RUN_RIGHT			13
#define MARIO_ANI_BIG_RUN_LEFT			14
#define MARIO_ANI_BIG_DRIFF_RIGHT		15
#define MARIO_ANI_BIG_DRIFF_LEFT		16

#define MARIO_ANI_RACCOON_IDLE_RIGHT	17
#define MARIO_ANI_RACCOON_IDLE_LEFT		18
#define MARIO_ANI_RACCOON_WALKING_RIGHT	19
#define MARIO_ANI_RACCOON_WALKING_LEFT	20
#define MARIO_ANI_RACCOON_JUMP_RIGHT	21
#define MARIO_ANI_RACCOON_JUMP_LEFT		22
#define MARIO_ANI_RACCOON_RUN_RIGHT		23
#define MARIO_ANI_RACCOON_RUN_LEFT		24
#define MARIO_ANI_RACCOON_DRIFF_RIGHT	25
#define MARIO_ANI_RACCOON_DRIFF_LEFT	26

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
	bool walkingRight = false;
	bool walkingLeft = false;
	bool isOnGround = true;
	float ax;
	int level;
	int untouchable;
	DWORD untouchable_start;
public:
	CMario() : CGameObject()
	{
		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
	}
	void CalVx(DWORD dt);
	bool canJump = 0;
	void LvlUp();
	void BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};