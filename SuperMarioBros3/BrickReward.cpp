#include "BrickReward.h"
#include "GameObject.h"
#include "Coin.h"

void CBrickReward::setReward(CGameObject* GObj) {
	this->reward = GObj;
	GObj->SetPosition(x + 2.0f, y - 16.0f);
}

void CBrickReward::showReward() {
	if (dynamic_cast<CCoin*>(this->reward)) {
		CCoin* coin = dynamic_cast<CCoin*>(this->reward);
		coin->showReward();
	}
}

void CBrickReward::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);

	// Simple fall down
	vy += BRICKREWARD_GRAVITY * dt;
	y += dy;

	if (y > y_start) {
		y = y_start;
		vy = 0;
	}
}

void CBrickReward::Render()
{
	int ani;
	ani = BRICKREWARD_ANI_IDLE;

	if (this->state != BRICKREWARD_STATE_IDLE)
		ani = BRICKREWARD_ANI_ACTIVED;

	int alpha = 255;
	animations[ani]->Render(x, y, alpha);
}

void CBrickReward::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BRICKREWARD_STATE_IDLE:
		vy = 0;
		break;
	case BRICKREWARD_STATE_JUMP:
		vy = -BRICKREWARD_JUMP_SPEED_Y;
		this->showReward();
		if (this->reward != NULL)
			reward->isDisable = false;
		break;
	}
}

void CBrickReward::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BRICKREWARD_BBOX_WIDTH;
	bottom = y + BRICKREWARD_BBOX_HEIGHT;
}
