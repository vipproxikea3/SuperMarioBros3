#pragma once
#include "GameObject.h"

#define GATE_ANI			0

#define GATE_TYPE_TOP		1
#define GATE_TYPE_BOTTOM	-1

#define GATE_BBOX_WIDTH		16
#define GATE_BBOX_HEIGHT	16

class CGate : public CGameObject
{
	float targetX, targetY;
	int targetZone;
	int type;
public:
	CGate(float x, float y, int targerZone,int type) : CGameObject()
	{
		targetX = x;
		targetY = y;
		targetZone = targerZone;
		this->type = type;
	}
	~CGate() {}
	float GetTargetX() { return targetX; }
	float GetTargetY() { return targetY; }
	int GetTargetZone() { return targetZone; }
	int GetType() { return type; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
