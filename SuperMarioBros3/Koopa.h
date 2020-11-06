#pragma once
#include "GameObject.h"

#define KOOPA_WALKING_SPEED				0.025f 

#define KOOPA_STATE_WALKING_RIGHT		100
#define KOOPA_STATE_WALKING_LEFT		200
#define KOOPA_STATE_DIE					300

#define KOOPA_ANI_WALKING_RIGHT			0
#define KOOPA_ANI_WALKING_LEFT			1

#define KOOPA_BBOX_WIDTH				16
#define KOOPA_BBOX_HEIGHT				26

class CKoopa : public CGameObject
{
	CGameObject* myShell;
	float limitL, limitR;
public:
	CKoopa() : CGameObject()
	{
		this->SetState(KOOPA_STATE_WALKING_LEFT);
	}
	void setMyShell(CGameObject* shell);
	void showShell();
	void setAactiveArea(float l, float r) { this->limitL = l; this->limitR = r; }
	void BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
