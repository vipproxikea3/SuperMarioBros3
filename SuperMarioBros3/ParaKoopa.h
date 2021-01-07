#pragma once
#include "GameObject.h"

#define PARAKOOPA_WALKING_SPEED				0.025f
#define PARAKOOPA_JUMP_SPEED				0.2f;
#define PARAKOOPA_GRAVITY					0.0005f
#define PARAKOOPA_SHELL_WALKING_SPEED		0.15f 
#define PARAKOOPA_SHELL_GRAVITY				0.0005f

#define PARAKOOPA_STATE_WALKING				100
#define	PARAKOOPA_SHELL_STATE_IDLE			200
#define PARAKOOPA_SHELL_STATE_WALKING		300
#define	PARAKOOPA_SHELL_STATE_BEHUG			400

#define PARAKOOPA_ANI_WALKING_RIGHT			0
#define PARAKOOPA_ANI_WALKING_LEFT			1
#define	PARAKOOPA_SHELL_ANI_IDLE			2
#define PARAKOOPA_SHELL_ANI_WALKING			3
#define PARAKOOPA_SHELL_ANI_BEHUG			4
#define PARAKOOPA_WING_LEFT_ANI_WALKING		5
#define PARAKOOPA_WING_LEFT_ANI_JUMP		6
#define PARAKOOPA_WING_RIGHT_ANI_WALKING	7
#define PARAKOOPA_WING_RIGHT_ANI_JUMP		8

#define PARAKOOPA_LEVEL_WING				1
#define	PARAKOOPA_LEVEL_KOOPA				2
#define	PARAKOOPA_LEVEL_SHELL				3

#define PARAKOOPA_BBOX_WIDTH				16
#define PARAKOOPA_BBOX_HEIGHT				26
#define PARAKOOPA_SHELL_BBOX_WIDTH			16
#define PARAKOOPA_SHELL_BBOX_HEIGHT			16

#define PARAKOOPA_SHELL_TIME				5000

class CParaKoopa : public CGameObject
{
	int level;
	float limitL, limitR;
	bool isHugging = false;
	DWORD shell_start;
public:
	CParaKoopa(float l, float r) : CGameObject()
	{
		this->SetState(PARAKOOPA_STATE_WALKING);
		this->setAactiveArea(l, r);
		this->SetLevel(PARAKOOPA_LEVEL_WING);
	}
	~CParaKoopa() {}
	void lvlDown();
	void setAactiveArea(float l, float r) { this->limitL = l; this->limitR = r; }
	void BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void ReSet();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
