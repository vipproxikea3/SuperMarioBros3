#include "BrickReward.h"
#include "GameObject.h"
#include "Coin.h"
#include "SuperMushroom.h"

void CBrickReward::setReward(CGameObject* GObj) {
	this->reward = GObj;
	GObj->isDisable = true;

	if (dynamic_cast<CCoin*>(this->reward)) {
		CCoin* coin = dynamic_cast<CCoin*>(this->reward);
		coin->SetPosition(x + 2.0f, y - 16.0f);
	}

	if (dynamic_cast<CSuperMushroom*>(this->reward)) {
		CSuperMushroom* mushroom = dynamic_cast<CSuperMushroom*>(this->reward);
		mushroom->SetPosition(x, y - 16.0f);
	}
}

void CBrickReward::showReward() {

	if (this->reward != NULL)
		this->reward->isDisable = false;

	if (dynamic_cast<CCoin*>(this->reward)) {
		CCoin* coin = dynamic_cast<CCoin*>(this->reward);
		coin->showReward();
	}

	if (dynamic_cast<CSuperMushroom*>(this->reward)) {
		CSuperMushroom* mushroom = dynamic_cast<CSuperMushroom*>(this->reward);
		mushroom->showReward();
	}
}

void CBrickReward::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (this->GetState() == BRICKREWARD_STATE_JUMP) {
		CGameObject::Update(dt);

		// Simple fall down
		vy += BRICKREWARD_GRAVITY * dt;
		y += dy;

		if (y > y_start) {
			y = y_start;
			vy = 0;
			this->SetState(BRICKREWARD_STATE_ACTIVED);
		}
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
		y = y_start;
		break;
	case BRICKREWARD_STATE_ACTIVED:
		vy = 0;
		y = y_start;
		break;
	case BRICKREWARD_STATE_JUMP:
		vy = -BRICKREWARD_JUMP_SPEED_Y;
		this->showReward();
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
