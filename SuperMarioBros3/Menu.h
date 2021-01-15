#pragma once
#include "GameObject.h"

#define MENU_ANI_OPTION_1	0
#define MENU_ANI_OPTION_2	1

#define MENU_STATE_OPTION_1	0
#define MENU_STATE_OPTION_2	100

#define MENU_OPTION_1		0
#define MENU_OPTION_2		1

#define MENU_BBOX_WIDTH		112
#define MENU_BBOX_HEIGHT	24

class CMenu : public CGameObject
{
	int current_option;
public:
	CMenu() : CGameObject()
	{
		this->SetState(MENU_STATE_OPTION_1);
	}
	~CMenu() {}
	void ChangeOption() {
		if (current_option == MENU_OPTION_1) {
			this->SetState(MENU_STATE_OPTION_2);
		}
		else {
			this->SetState(MENU_STATE_OPTION_1);
		}
	}
	void PlayOption();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
