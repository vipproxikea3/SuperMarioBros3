#pragma once
#include "GameObject.h"
#include "ParaGoomba.h"

#define KOOPA_WALKING_SPEED				0.025f
#define KOOPA_GRAVITY					0.0005f
#define SHELL_WALKING_SPEED				0.15f 
#define SHELL_GRAVITY					0.0005f

#define KOOPA_STATE_WALKING				100
#define	SHELL_STATE_IDLE				200
#define SHELL_STATE_WALKING				300
#define	SHELL_STATE_BEHUG				400

#define KOOPA_ANI_WALKING_RIGHT			0
#define KOOPA_ANI_WALKING_LEFT			1
#define	SHELL_ANI_IDLE					2
#define SHELL_ANI_WALKING				3
#define SHELL_ANI_BEHUG					4

#define	KOOPA_LEVEL_KOOPA				1
#define	KOOPA_LEVEL_SHELL				2

#define KOOPA_BBOX_WIDTH				16
#define KOOPA_BBOX_HEIGHT				26
#define SHELL_BBOX_WIDTH				16
#define SHELL_BBOX_HEIGHT				16

#define SHELL_TIME 10000

class CKoopa : public CGameObject
{
	int level;
	float limitL, limitR;
	bool isHugging = false;
	DWORD shell_start;
public:
	CKoopa(float l, float r) : CGameObject()
	{
		this->SetState(KOOPA_STATE_WALKING);
		this->setAactiveArea(l, r);
		this->SetLevel(KOOPA_LEVEL_KOOPA);
	}
	~CKoopa() {}
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
