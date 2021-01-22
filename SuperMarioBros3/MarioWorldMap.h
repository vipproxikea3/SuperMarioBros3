#pragma once
#include "GameObject.h"

#define MARIOWORLDMAP_WALKING_SPEED			0.075f

#define MARIOWORLDMAP_ANI_SMALL				0
#define MARIOWORLDMAP_ANI_RACCOON			1
#define MARIOWORLDMAP_ANI_BIG				2
#define MARIOWORLDMAP_ANI_FIRE				3

#define MARIOWORLDMAP_LEVEL_SMALL			1
#define MARIOWORLDMAP_LEVEL_RACCOON			2
#define MARIOWORLDMAP_LEVEL_BIG				3
#define MARIOWORLDMAP_LEVEL_FIRE			4

#define MARIOWORLDMAP_STATE_IDLE			100
#define MARIOWORLDMAP_STATE_WALKING_LEFT	200
#define MARIOWORLDMAP_STATE_WALKING_RIGHT	300
#define MARIOWORLDMAP_STATE_WALKING_TOP		400
#define MARIOWORLDMAP_STATE_WALKING_BOTTOM	500

#define MARIOWORLDMAP_BBOX_WIDTH		16
#define MARIOWORLDMAP_BBOX_HEIGHT		16

class CMarioWorldMap : public CGameObject
{
	float lastPosX, lastPosY, curPosX, curPosY;
	bool lastCanLeft, lastCanTop, lastCanRight, lastCanBot;
	bool curCanLeft, curCanTop, curCanRight, curCanBot;

	float lLeft = -1000.0f;
	float lRight = 1000.0f;
	float lTop = -1000.0f;
	float lBottom = 1000.0f;
	int sceneId = -1;
public:
	bool canUp = false;
	bool canDown = false;
	bool canRight = false;
	bool canLeft = false;

	int level;

	CMarioWorldMap() : CGameObject()
	{
		level = MARIOWORLDMAP_LEVEL_SMALL;
		this->SetState(MARIOWORLDMAP_STATE_IDLE);
		canRight = true;
	}
	~CMarioWorldMap() {}
	void SetCanNotControl() {
		canUp = false;
		canDown = false;
		canRight = false;
		canLeft = false;
	}
	void SetDefaultLimit() {
		lLeft = -1000.0f;
		lRight = 1000.0f;
		lTop = -1000.0f;
		lBottom = 1000.0f;
	}
	void SetLimit(float l, float t, float r, float b) {
		this->lLeft = l;
		this->lTop = t;
		this->lRight = r;
		this->lBottom = b;
	}
	void SetControl(bool l, bool t, bool r, bool b) {
		canLeft = l;
		canUp = t;
		canRight = r;
		canDown = b;		
	}
	void SwitchScene() {
		CGame* game = CGame::GetInstance();
		if (sceneId != 0) {
			game->SwitchScene(sceneId);
		}
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
