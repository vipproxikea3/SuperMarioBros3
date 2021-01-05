#pragma once
#include "GameObject.h"
#include "Mario.h"

#define SUPERLEAF_JUMP_SPEED_Y	0.03f
#define SUPERLEAF_FALL_SPEED_X	0.025f
#define SUPERLEAF_GRAVITY		0.00003f

#define SUPERLEAF_STATE_IDLE	0
#define SUPERLEAF_STATE_JUMP	100
#define SUPERLEAF_STATE_FALL	200

#define SUPERLEAF_ANI_IDLE		0

#define SUPERLEAF_BBOX_WIDTH	16
#define SUPERLEAF_BBOX_HEIGHT	14

class CSuperLeaf : public CGameObject
{
public:
	CSuperLeaf() : CGameObject()
	{
		this->SetState(SUPERLEAF_STATE_IDLE);
	}
	~CSuperLeaf() {}
	void Jump();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
