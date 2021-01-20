#pragma once
#include "GameObject.h"

#define TAILATTACKEFFECT_ANI_IDLE		0

#define TAILATTACKEFFECT_BBOX_WIDTH		16
#define TAILATTACKEFFECT_BBOX_HEIGHT	16

#define TAILATTACKEFFECT_TIME			200

class CTailAttackEffect : public CGameObject
{
	DWORD CreateAt;
public:
	CTailAttackEffect() : CGameObject()
	{
		CreateAt = GetTickCount64();
	}
	~CTailAttackEffect() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};