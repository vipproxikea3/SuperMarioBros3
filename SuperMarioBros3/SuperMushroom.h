#pragma once
#include "GameObject.h"
#include "Mario.h"

#define SUPERMUSHROOM_WALKING_SPEED		0.02f 
#define SUPERMUSHROOM_GRAVITY			0.0008f

#define SUPERMUSHROOM_STATE_IDLE		0
#define SUPERMUSHROOM_STATE_WALKING		100

#define SUPERMUSHROOM_TYPE_LEVEL		0
#define SUPERMUSHROOM_TYPE_LIFE			1

#define SUPERMUSHROOM_LEVEL_ANI			0
#define SUPERMUSHROOM_LIFE_ANI			1

#define SUPERMUSHROOM_BBOX_WIDTH		16
#define SUPERMUSHROOM_BBOX_HEIGHT		16


class CSuperMushroom : public CGameObject
{
	int type;
public:
	CSuperMushroom(int type) : CGameObject()
	{
		this->SetState(SUPERMUSHROOM_STATE_WALKING);
		this->type = type;
	}
	~CSuperMushroom() {}
	void BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	int GetType() { return type; }
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
