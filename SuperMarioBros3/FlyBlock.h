#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Backup.h"

#define FLYBLOCK_WALKING_SPEED	0.025f
#define FLYBLOCK_FALL_SPEED		0.075f

#define FLYBLOCK_STATE_WALKING	0
#define FLYBLOCK_STATE_FALL		100

#define FLYBLOCK_ANI_IDLE		0

#define FLYBLOCK_BBOX_WIDTH		48
#define FLYBLOCK_BBOX_HEIGHT	16

class CFlyBlock : public CGameObject
{
public:
	CFlyBlock() : CGameObject()
	{
		this->SetState(FLYBLOCK_STATE_WALKING);
	}
	~CFlyBlock() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
