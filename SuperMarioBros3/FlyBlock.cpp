#include "FlyBlock.h"

void CFlyBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!this->isDisable && this->IsInCamera()) {
		// Calculate dx, dy 
		CGameObject::Update(dt);

		if (this->state == FLYBLOCK_STATE_WALKING) {
			x += dx;
		}
		else {
			y += dy;
		}
	}
}

void CFlyBlock::Render()
{
	int ani;
	ani = FLYBLOCK_ANI_IDLE;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CFlyBlock::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case FLYBLOCK_STATE_WALKING:
		vy = 0;
		vx = -FLYBLOCK_WALKING_SPEED;
		break;
	case FLYBLOCK_STATE_FALL:
		vx = 0;
		vy = FLYBLOCK_FALL_SPEED;
		break;
	}
}

void CFlyBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FLYBLOCK_BBOX_WIDTH;
	bottom = y + FLYBLOCK_BBOX_HEIGHT;
}