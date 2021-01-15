#pragma once
#pragma once
#include "GameObject.h"

#define STATION_ANI_IDLE		0

#define STATION_BBOX_WIDTH		16
#define STATION_BBOX_HEIGHT		16

class CStation : public CGameObject
{
public:
	int targetScene = -1;
	bool canUp = false;
	bool canDown = false;
	bool canRight = false;
	bool canLeft = false;

	CStation(bool l, bool t, bool r, bool b, int targetScene) : CGameObject()
	{
		canUp = t;
		canDown = b;
		canRight = r;
		canLeft = l;
		this->targetScene = targetScene;
	}
	~CStation() {}
	int GetTargetScene() { return targetScene; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
