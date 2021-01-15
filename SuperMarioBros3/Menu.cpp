#include "Menu.h"

void CMenu::PlayOption() {
	switch (current_option)
	{
	case MENU_OPTION_1:
		CGame::GetInstance()->SwitchScene(2);
		break;
	}
}

void CMenu::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
}

void CMenu::Render()
{
	int ani;

	switch (state)
	{
	case MENU_STATE_OPTION_1:
		ani = MENU_ANI_OPTION_1;
		break;
	case MENU_STATE_OPTION_2:
		ani = MENU_ANI_OPTION_2;
		break;
	}

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CMenu::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MENU_STATE_OPTION_1:
		this->current_option = MENU_OPTION_1;
		break;
	case MENU_STATE_OPTION_2:
		this->current_option = MENU_OPTION_2;
		break;
	}
}

void CMenu::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MENU_BBOX_WIDTH;
	bottom = y + MENU_BBOX_HEIGHT;
}