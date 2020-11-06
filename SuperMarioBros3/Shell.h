#pragma once
#include "GameObject.h"

#define SHELL_WALKING_SPEED				0.125f 
#define SHELL_GRAVITY					0.0005f

#define SHELL_STATE_IDLE				0
#define SHELL_STATE_WALKING_RIGHT		100
#define SHELL_STATE_WALKING_LEFT		200

#define SHELL_ANI_IDLE					0
#define SHELL_ANI_SMALL_WALKING			1

#define SHELL_SMALL_BBOX_WIDTH			16
#define SHELL_SMALL_BBOX_HEIGHT			16

class CShell : public CGameObject
{
public:
	CShell() : CGameObject()
	{
		this->SetState(SHELL_STATE_WALKING_RIGHT);
	}
	void BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
