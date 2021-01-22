#pragma once
#include "GameObject.h"

#define CURTAIN_ANI_IDLE		0

#define CURTAIN_SPEED_Y			0.05f;

#define CURTAIN_BBOX_WIDTH		256
#define CURTAIN_BBOX_HEIGHT		182

class CCurtain : public CGameObject
{
public:
	CCurtain() : CGameObject()
	{
		vy = -CURTAIN_SPEED_Y;
	}
	~CCurtain() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
