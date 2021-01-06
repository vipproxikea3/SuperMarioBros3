#pragma once
#include "GameObject.h"

#define HUD_ANI_BG				0
#define HUD_ANI_RUNSTACK_NORMAL	1
#define HUD_ANI_RUNSTACK_MAX	2

#define RUNSTACK_NORMAL_POS_X	88
#define RUNSTACK_NORMAL_POS_Y	13
#define RUNSTACK_NORMAL_WIDTH	8
#define RUNSTACK_MAX_POS_X		137
#define RUNSTACK_MAX_POS_Y		13

class CHUD : public CGameObject
{
public:
	CHUD() : CGameObject()
	{
	}
	~CHUD() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	void RenderRunSpeedStack();

	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
