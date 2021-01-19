#pragma once
#include "GameObject.h"
#include "ParaGoomba.h"
#include "BreakBlock.h"

#define KOOPA_WALKING_SPEED				0.025f
#define KOOPA_GRAVITY					0.0005f
#define SHELL_WALKING_SPEED				0.15f 
#define SHELL_GRAVITY					0.0005f
#define SHELL_JUMP_DEFLECT_SPEED		0.25f

#define KOOPA_TYPE_GREEN				0
#define KOOPA_TYPE_RED					1

#define KOOPA_STATE_WALKING_LEFT		100
#define KOOPA_STATE_WALKING_RIGHT		200
#define	SHELL_STATE_IDLE				300
#define SHELL_STATE_WALKING_LEFT		400
#define SHELL_STATE_WALKING_RIGHT		500
#define	SHELL_STATE_BEHUG				600
#define	SHELL_STATE_OVERTURN			700
#define SHELL_STATE_DIE					800

#define KOOPA_GREEN_ANI_WALKING_RIGHT	0
#define KOOPA_GREEN_ANI_WALKING_LEFT	1
#define	SHELL_GREEN_ANI_IDLE			2
#define SHELL_GREEN_ANI_WALKING			3
#define SHELL_GREEN_ANI_BEHUG			4
#define SHELL_GREEN_ANI_OVERTURN		5
#define KOOPA_RED_ANI_WALKING_RIGHT		6
#define KOOPA_RED_ANI_WALKING_LEFT		7
#define	SHELL_RED_ANI_IDLE				8
#define SHELL_RED_ANI_WALKING			9
#define SHELL_RED_ANI_BEHUG				10
#define SHELL_RED_ANI_OVERTURN			11

#define	KOOPA_LEVEL_KOOPA				1
#define	KOOPA_LEVEL_SHELL				2

#define KOOPA_BBOX_WIDTH				16
#define KOOPA_BBOX_HEIGHT				27
#define SHELL_BBOX_WIDTH				16
#define SHELL_BBOX_HEIGHT				16

#define SHELL_TIME 10000

class CKoopa : public CGameObject
{
	int type;
	int level;
	bool isHugging = false;
	DWORD shell_start;
public:
	CKoopa(int type) : CGameObject()
	{
		this->type = type;
		this->SetState(KOOPA_STATE_WALKING_LEFT);
		this->SetLevel(KOOPA_LEVEL_KOOPA);
	}
	~CKoopa() {}
	void lvlDown();
	void BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	int GetType() { return this->type; }
	void ReSet();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
