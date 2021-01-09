#pragma once
#pragma once
#include "GameObject.h"

#define VENUSFIRETRAPBULLET_ANI			0

#define VENUSFIRETRAPBULLET_BBOX_WIDTH	8
#define VENUSFIRETRAPBULLET_BBOX_HEIGHT	8

class CVenusFireTrapBullet : public CGameObject
{
public:
	CVenusFireTrapBullet() : CGameObject()
	{
	}
	~CVenusFireTrapBullet() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

