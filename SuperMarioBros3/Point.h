#pragma once
#include "GameObject.h"

#define POINT_TYPE_LVLUP	0
#define POINT_TYPE_100		1

#define POINT_ANI_LVLUP		0
#define POINT_ANI_100		1

#define POINT_BBOX_WIDTH	16
#define POINT_BBOX_HEIGHT	8

#define TIME_ALIVE			250

class CPoint : public CGameObject
{
	float createdAt;
	int type;
public:
	CPoint(int type) : CGameObject()
	{
		createdAt = GetTickCount64();
		this->type = type;

		CGame* game = CGame::GetInstance();
		switch (type)
		{
		case POINT_TYPE_100:
			game->PushPoint(100);
			break;
		}
	}
	~CPoint() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
