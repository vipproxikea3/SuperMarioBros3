#pragma once
#include "GameObject.h"

#define HUD_ANI_BG				0
#define HUD_ANI_RUNSTACK_NORMAL	1
#define HUD_ANI_RUNSTACK_MAX	2

#define HUD_ANI_NUMBER_0		3
#define HUD_ANI_NUMBER_1		4
#define HUD_ANI_NUMBER_2		5
#define HUD_ANI_NUMBER_3		6
#define HUD_ANI_NUMBER_4		7
#define HUD_ANI_NUMBER_5		8
#define HUD_ANI_NUMBER_6		9
#define HUD_ANI_NUMBER_7		10
#define HUD_ANI_NUMBER_8		11
#define HUD_ANI_NUMBER_9		12

#define RUNSTACK_NORMAL_POS_X	88
#define RUNSTACK_NORMAL_POS_Y	13
#define RUNSTACK_NORMAL_WIDTH	8
#define RUNSTACK_MAX_POS_X		137
#define RUNSTACK_MAX_POS_Y		13

#define COIN_POS_X				168
#define COIN_POS_Y				13

#define TIME_POS_X				160
#define TIME_POS_Y				21

#define LIFE_POS_X				65
#define LIFE_POS_Y				21

#define POINT_POS_X				88
#define POINT_POS_Y				21

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
	void RenderCoin();
	void RenderRemainingTime();
	void RenderLifeStack();
	void RenderPoint();

	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
