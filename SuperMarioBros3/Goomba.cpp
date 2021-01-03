#include "Goomba.h"
#include "Block.h"
#include "Game.h"

CGoomba::CGoomba(float l, float r) {
	this->SetState(GOOMBA_STATE_WALKING);
	this->SetActiveArea(l, r);
}

CGoomba::~CGoomba() {}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ReSet();
	if (!this->isDisable) {
		CGameObject::Update(dt, coObjects);
		vy += GOOMBA_GRAVITY * dt;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		if ((this->state == GOOMBA_STATE_DIE_Y || this->state == GOOMBA_STATE_DIE_X) && GetTickCount64() - die_start > DIE_TIME) {
			this->isDisable = true;
		}

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

				if (dynamic_cast<CBlock*>(e->obj)) {
					CBlock* block = dynamic_cast<CBlock*>(e->obj);

					if (e->nx == -1 && block->isBlockLeft()) {
						this->vx = -vx;
						this->x = x0 + min_tx * this->dx + nx * 0.4f;
					}

					if (e->nx == 1 && block->isBlockRight()) {
						this->vx = -vx;
						this->x = x0 + min_tx * this->dx + nx * 0.4f;
					}

					if (e->ny == -1 && block->isBlockTop()) {
						this->vy = 0;
						this->y = y0 + min_ty * this->dy + ny * 0.4f;
					}

					if (e->ny == 1 && block->isBlockBottom()) {
						this->vy = 0;
						this->y = y0 + min_ty * this->dy + ny * 0.4f;
					}
				}
			}
		}

		if (this->x <= this->lLimit) {
			this->x = this->lLimit;
			this->vx = -this->vx;
		}
		if (this->x >= this->rLimit - GOOMBA_BBOX_WIDTH) {
			this->x = this->rLimit - GOOMBA_BBOX_WIDTH;
			this->vx = -this->vx;
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	
	switch (this->GetState())
	{
	case GOOMBA_STATE_DIE_Y:
		ani = GOOMBA_ANI_DIE_Y;
		break;
	case GOOMBA_STATE_DIE_X:
		ani = GOOMBA_ANI_DIE_X;
		break;
	default:
		ani = GOOMBA_ANI_WALKING;
		break;
	}

	animation_set->at(ani)->Render(x, y, 255);
}

void CGoomba::ReSet() {
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);
	CGame* game = CGame::GetInstance();
	if (!game->IsInCamera(l, t, r, b)) {
		this->isDisable = true;
		this->isReadyReset = false;
		this->SetPosition(this->x_start, this->y_start);
		this->SetState(GOOMBA_STATE_WALKING);
	}
	if (!game->IsInCamera(this->x_start, this->y_start, this->x_start + r - l, this->y_start + b - t)) {
		if (this->isDisable) {
			this->isReadyReset = true;
		}
	}
	if (game->IsInCamera(l, t, r, b)) {
		if (this->isReadyReset)
			this->isDisable = false;
	}
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE_X:
		vy = -GOOMBA_DIE_DEFLECT_SPEED;
		die_start = GetTickCount64();
		break;
	case GOOMBA_STATE_DIE_Y:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
		die_start = GetTickCount64();
		break;
	case GOOMBA_STATE_WALKING:
		die_start = NULL;
		vx = -GOOMBA_WALKING_SPEED;
		vy = 0;
	}
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (this->GetState())
	{
	case GOOMBA_STATE_WALKING:
		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;
		bottom = y + GOOMBA_BBOX_HEIGHT;
		break;
	default:
		top = y;
		left = x;
		bottom = y;
		right = x;
		break;
	}
}