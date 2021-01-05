#include "SuperLeaf.h"

void CSuperLeaf::Jump() {
	this->SetState(SUPERLEAF_STATE_JUMP);
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (this->state == SUPERLEAF_STATE_JUMP) {
		// Calculate dx, dy 
		CGameObject::Update(dt);

		// Simple fall down
		vy += SUPERLEAF_GRAVITY * dt;

		y += dy;

		if (vy >= 0) this->SetState(SUPERLEAF_STATE_FALL);
	}
	if (this->state == SUPERLEAF_STATE_FALL) {
		// Calculate dx, dy 
		CGameObject::Update(dt);

		// Simple fall down
		if (vx < 0)
			vy += SUPERLEAF_GRAVITY * dt;
		else vy = 0;

		y += dy;
		x += dx;

		if (x <= this->x_start - SUPERLEAF_BBOX_WIDTH)
		{
			x = this->x_start - SUPERLEAF_BBOX_WIDTH;
			vx = SUPERLEAF_FALL_SPEED_X;
		}
		if (x >= this->x_start + SUPERLEAF_BBOX_WIDTH * 2) {
			x = this->x_start + SUPERLEAF_BBOX_WIDTH * 2;
			vx = -SUPERLEAF_FALL_SPEED_X;
		}
	}
}

void CSuperLeaf::Render()
{
	int ani;
	ani = SUPERLEAF_ANI_IDLE;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CSuperLeaf::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SUPERLEAF_STATE_IDLE:
		vy = 0;
		vx = 0;
		break;
	case SUPERLEAF_STATE_JUMP:
		vy = -SUPERLEAF_JUMP_SPEED_Y;
		vx = 0;
		break;
	case SUPERLEAF_STATE_FALL:
		vy = 0;
		vx = -SUPERLEAF_FALL_SPEED_X;
		break;
	}
}

void CSuperLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SUPERLEAF_BBOX_WIDTH;
	bottom = y + SUPERLEAF_BBOX_HEIGHT;
}