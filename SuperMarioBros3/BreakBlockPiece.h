#pragma once
#include "GameObject.h"

#define BREAKBLOCKPIECE_HIGH_JUMP_SPEED			0.25f
#define BREAKBLOCKPIECE_LOW_JUMP_SPEED			0.15f
#define BREAKBLOCKPIECE_WALKING_SPEED			0.05f
#define BREAKBLOCKPIECE_GRAVITY					0.001f

#define BREAKBLOCKPIECE_TYPE_HIGH_LEFT			0
#define BREAKBLOCKPIECE_TYPE_HIGH_RIGHT			1
#define BREAKBLOCKPIECE_TYPE_LOW_LEFT			2
#define BREAKBLOCKPIECE_TYPE_LOW_RIGHT			3

#define BREAKBLOCKPIECE_STATE_HIGH_JUMP_LEFT	0
#define BREAKBLOCKPIECE_STATE_HIGH_JUMP_RIGHT	100
#define BREAKBLOCKPIECE_STATE_LOW_JUMP_LEFT		200
#define BREAKBLOCKPIECE_STATE_LOW_JUMP_RIGHT	300

#define BREAKBLOCKPIECE_ANI						0

#define BREAKBLOCKPIECE_BBOX_WIDTH				8
#define BREAKBLOCKPIECE_BBOX_HEIGHT				8

#define TIME_ALIVE								500

class CBreakBlockPiece : public CGameObject
{
	float createdAt;
	int type;
public:
	CBreakBlockPiece(int type) : CGameObject()
	{
		createdAt = GetTickCount64();
		this->type = type;
		switch (type)
		{
		case BREAKBLOCKPIECE_TYPE_HIGH_LEFT:
			this->SetState(BREAKBLOCKPIECE_STATE_HIGH_JUMP_LEFT);
			break;
		case BREAKBLOCKPIECE_TYPE_HIGH_RIGHT:
			this->SetState(BREAKBLOCKPIECE_STATE_HIGH_JUMP_RIGHT);
			break;
		case BREAKBLOCKPIECE_TYPE_LOW_LEFT:
			this->SetState(BREAKBLOCKPIECE_STATE_LOW_JUMP_LEFT);
			break;
		case BREAKBLOCKPIECE_TYPE_LOW_RIGHT:
			this->SetState(BREAKBLOCKPIECE_STATE_LOW_JUMP_RIGHT);
			break;
		}
	}
	~CBreakBlockPiece() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
