#include "Coin.h"

void CCoin::showReward() {
	this->SetState(COIN_STATE_JUMP);
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (this->state == COIN_STATE_JUMP) {
		// Calculate dx, dy 
		CGameObject::Update(dt);

		// Simple fall down
		vy += COIN_GRAVITY * dt;

		y += dy;

		if (y > y_start) isDisable = true;
	}
}

void CCoin::Render()
{
	int ani;
	ani = COIN_ANI_IDLE;

	int alpha = 255;
	animations[ani]->Render(x, y, alpha);
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case COIN_STATE_IDLE:
			vy = 0;
		break;
	case COIN_STATE_JUMP:
		vy = -COIN_JUMP_SPEED_Y;
		break;
	}
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + COIN_BBOX_WIDTH;
	bottom = y + COIN_BBOX_HEIGHT;
}