#pragma once
#include "GameObject.h"

#define MARIOTRANSFORMEFFECT_ANI_IDLE		0

#define MARIOTRANSFORMEFFECT_BBOX_WIDTH		16
#define MARIOTRANSFORMEFFECT_BBOX_HEIGHT	16

#define MARIOTRANSFORMEFFECT_TIME			300

class CMarioTransformEffect : public CGameObject
{
	DWORD CreateAt;
public:
	CMarioTransformEffect() : CGameObject()
	{
		CreateAt = GetTickCount64();
	}
	~CMarioTransformEffect() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};