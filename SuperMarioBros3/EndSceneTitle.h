#pragma once
#include "GameObject.h"
#include "Utils.h"

#define ENDSCENETITLE_ANI_BG		0
#define ENDSCENETITLE_ANI_MUSHROOM	1
#define ENDSCENETITLE_ANI_FLOWER	2
#define ENDSCENETITLE_ANI_STAR		3

#define ENDSCENETITLE_TIME_SHOW		1500

#define ENDSCENETITLE_TYPE_MUSHROOM	0
#define ENDSCENETITLE_TYPE_FLOWER	100
#define ENDSCENETITLE_TYPE_STAR		200

#define ENDSCENETITLE_CARD_POS_X	127
#define ENDSCENETITLE_CARD_POS_Y	23

#define ENDSCENETITLE_BBOX_WIDTH	146
#define ENDSCENETITLE_BBOX_HEIGHT	44

class CEndSceneTitle : public CGameObject
{
	int type;
	bool showing;
	DWORD start_show = NULL;
public:
	CEndSceneTitle(int type) : CGameObject()
	{
		this->type = type;
		showing = false;
		start_show = GetTickCount64();
	}
	~CEndSceneTitle() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
