#pragma once
#include "GameObject.h"
#include "Mario.h"

#define PIRANHAPLANT_SPEED_Y		0.02f

#define PIRANHAPLANT_STATE_IDLE		0
#define PIRANHAPLANT_STATE_UP		100
#define PIRANHAPLANT_STATE_BITE		200
#define PIRANHAPLANT_STATE_DOWN		300
#define PIRANHAPLANT_STATE_DIE		400

#define PIRANHAPLANT_ANI			0

#define PIRANHAPLANT_BBOX_WIDTH		16
#define PIRANHAPLANT_BBOX_HEIGHT	32

#define PIRANHAPLANT_RISE_HEIGHT	24

#define PIRANHAPLANT_TIME_BITE		500
#define PIRANHAPLANT_COOLDOWNS		1000

#define PIRANHAPLANT_SAFE_ZONE		8

class CPiranhaPlant : public CGameObject
{
	DWORD last_rise;
	DWORD start_bite;
public:
	CPiranhaPlant() : CGameObject()
	{
		this->SetState(PIRANHAPLANT_STATE_IDLE);
		last_rise = GetTickCount64();
	}
	~CPiranhaPlant() {}
	bool CheckMarioInActiveZone();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
