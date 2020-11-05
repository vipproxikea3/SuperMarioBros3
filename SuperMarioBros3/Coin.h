#pragma once
#include "GameObject.h"

#define COIN_ANI_IDLE 0

#define COIN_BBOX_WIDTH  10
#define COIN_BBOX_HEIGHT 16

class CCoin : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
