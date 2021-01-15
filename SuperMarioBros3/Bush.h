#pragma once
#include "GameObject.h"

#define BUSH_ANI_IDLE			0

#define BUSH_BBOX_WIDTH			16
#define BUSH_BBOX_HEIGHT		16

class CBush : public CGameObject
{
public:
	CBush() : CGameObject()
	{
	}
	~CBush() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
