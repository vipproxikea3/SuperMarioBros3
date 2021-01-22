#pragma once
#include "GameObject.h"

#define CAMERAMAN_WALKING_SPEED	0.02f

class CCameraMan : public CGameObject
{
public:
	CCameraMan() : CGameObject()
	{
		vx = CAMERAMAN_WALKING_SPEED;
	}
	~CCameraMan() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
