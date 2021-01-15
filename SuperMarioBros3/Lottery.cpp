#include "Lottery.h"

void CLottery::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!this->isDisable) {
		if (GetTickCount64() - last_change >= LOTTERY_TIME_CHANGE) {
			switch (state)
			{
			case LOTTERY_STATE_MUSHROOM:
				this->SetState(LOTTERY_STATE_FLOWER);
				break;
			case LOTTERY_STATE_FLOWER:
				this->SetState(LOTTERY_STATE_STAR);
				break;
			case LOTTERY_STATE_STAR:
				this->SetState(LOTTERY_STATE_MUSHROOM);
				break;
			}
		}
	}
}

void CLottery::Render()
{
	int ani;

	switch (state)
	{
	case LOTTERY_STATE_MUSHROOM:
		ani = LOTTERY_ANI_MUSHROOM;
		break;
	case LOTTERY_STATE_FLOWER:
		ani = LOTTERY_ANI_FLOWER;
		break;
	case LOTTERY_STATE_STAR:
		ani = LOTTERY_ANI_STAR;
		break;
	}

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CLottery::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case LOTTERY_STATE_MUSHROOM:
		last_change = GetTickCount64();
		break;
	case LOTTERY_STATE_FLOWER:
		last_change = GetTickCount64();
		break;
	case LOTTERY_STATE_STAR:
		last_change = GetTickCount64();
		break;
	}
}

void CLottery::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + LOTTERY_BBOX_WIDTH;
	bottom = y + LOTTERY_BBOX_HEIGHT;
}