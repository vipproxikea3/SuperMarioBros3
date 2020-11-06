#pragma once
#include "GameObject.h"

#define SUPERMUSHROOM_WALKING_SPEED				0.02f 
#define SUPERMUSHROOM_GRAVITY					0.0008f

#define SUPERMUSHROOM_STATE_IDLE				0
#define SUPERMUSHROOM_STATE_WALKING_RIGHT		100
#define SUPERMUSHROOM_STATE_WALKING_LEFT		200

#define SUPERMUSHROOM_ANI_SMALL_IDLE			0

#define SUPERMUSHROOM_BBOX_WIDTH				16
#define SUPERMUSHROOM_BBOX_HEIGHT				15


class CSuperMushroom : public CGameObject
{
public:
	CSuperMushroom() : CGameObject()
	{
		this->SetState(SUPERMUSHROOM_STATE_IDLE);
	}
	void showReward();
	void BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};