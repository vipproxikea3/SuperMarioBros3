#include "SuperLeaf.h"

void CSuperLeaf::Jump() {
	this->SetState(SUPERLEAF_STATE_JUMP);
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	// Calculate dx, dy 
	CGameObject::Update(dt);

	if (this->state == SUPERLEAF_STATE_JUMP) {
		// Simple fall down
		vy += SUPERLEAF_GRAVITY * dt;

		y += dy;

		if (vy > 0) this->SetState(SUPERLEAF_STATE_FALL);
	}
	if (this->state == SUPERLEAF_STATE_FALL) {
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
		if (x >= this->x_start + SUPERLEAF_BBOX_WIDTH) {
			x = this->x_start + SUPERLEAF_BBOX_WIDTH;
			vx = -SUPERLEAF_FALL_SPEED_X;
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		float x0 = x;
		float y0 = y;

		x = x0 + dx;
		y = y0 + dy;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];

			//MARIO
			if (dynamic_cast<CMario*>(e->obj)) {
				CMario* mario = dynamic_cast<CMario*>(e->obj);
				this->isDisable = true;
				if (mario->GetLevel() == MARIO_LEVEL_BIG)
					mario->LvlUp();
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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