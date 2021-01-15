#pragma once
#include "GameObject.h"

#define LOGO_ANI_IDLE			0

#define LOGO_BBOX_WIDTH			42
#define LOGO_BBOX_HEIGHT		41

class CLogo : public CGameObject
{
public:
	CLogo() : CGameObject()
	{
	}
	~CLogo() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
