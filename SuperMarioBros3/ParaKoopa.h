#pragma once
#include "GameObject.h"

#define PARAKOOPA_WALKING_SPEED				0.025f
#define PARAKOOPA_JUMP_SPEED				0.2f;
#define PARAKOOPA_GRAVITY					0.0005f
#define PARAKOOPA_SHELL_WALKING_SPEED		0.15f 
#define PARAKOOPA_SHELL_GRAVITY				0.0005f
#define PARAKOOPA_SHELL_JUMP_DEFLECT_SPEED	0.25f
#define PARAKOOPA_DIE_DEFLECT_SPEED			0.1f

#define PARAKOOPA_TYPE_GREEN					0
#define PARAKOOPA_TYPE_RED						1

#define PARAKOOPA_STATE_WALKING_LEFT		100
#define PARAKOOPA_STATE_WALKING_RIGHT		200
#define	PARAKOOPA_SHELL_STATE_IDLE			300
#define PARAKOOPA_SHELL_STATE_WALKING_LEFT	400
#define PARAKOOPA_SHELL_STATE_WALKING_RIGHT	500
#define	PARAKOOPA_SHELL_STATE_BEHUG			600
#define	PARAKOOPA_SHELL_STATE_OVERTURN		700
#define PARAKOOPA_STATE_DIE					800

#define PARAKOOPA_GREEN_ANI_WALKING_RIGHT	0
#define PARAKOOPA_GREEN_ANI_WALKING_LEFT	1
#define	PARAKOOPA_GREEN_SHELL_ANI_IDLE		2
#define PARAKOOPA_GREEN_SHELL_ANI_WALKING	3
#define PARAKOOPA_GREEN_SHELL_ANI_BEHUG		4
#define PARAKOOPA_GREEN_SHELL_ANI_OVERTURN	5
#define PARAKOOPA_GREEN_ANI_DIE				6
#define PARAKOOPA_RED_ANI_WALKING_RIGHT		7
#define PARAKOOPA_RED_ANI_WALKING_LEFT		8
#define	PARAKOOPA_RED_SHELL_ANI_IDLE		9
#define PARAKOOPA_RED_SHELL_ANI_WALKING		10
#define PARAKOOPA_RED_SHELL_ANI_BEHUG		11
#define PARAKOOPA_RED_SHELL_ANI_OVERTURN	12
#define PARAKOOPA_RED_ANI_DIE				13
#define PARAKOOPA_WING_LEFT_ANI_WALKING		14
#define PARAKOOPA_WING_LEFT_ANI_JUMP		15
#define PARAKOOPA_WING_RIGHT_ANI_WALKING	16
#define PARAKOOPA_WING_RIGHT_ANI_JUMP		17

#define PARAKOOPA_LEVEL_WING				1
#define	PARAKOOPA_LEVEL_KOOPA				2
#define	PARAKOOPA_LEVEL_SHELL				3

#define PARAKOOPA_BBOX_WIDTH				16
#define PARAKOOPA_BBOX_HEIGHT				27
#define PARAKOOPA_SHELL_BBOX_WIDTH			16
#define PARAKOOPA_SHELL_BBOX_HEIGHT			16

#define PARAKOOPA_SHELL_TIME				5000

class CParaKoopa : public CGameObject
{
	int type;
	int level;
	bool isHugging = false;
	DWORD shell_start;
public:
	CParaKoopa(int type) : CGameObject()
	{
		this->type = type;
		this->SetState(PARAKOOPA_STATE_WALKING_LEFT);
		this->SetLevel(PARAKOOPA_LEVEL_WING);
	}
	~CParaKoopa() {}
	void lvlDown();
	void BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void ReSet();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
