#include "Goomba.h"
#include "Block.h"
#include "Game.h"



//void CGoomba::Revival() {
//	if (this->state == GOOMBA_STATE_DIE) {
//		CGame* game = CGame::GetInstance();
//		float cam_X, cam_Y;
//		game->GetCamPos(cam_X, cam_Y);
//
//		float screen_width;
//		screen_width = game->GetScreenWidth();
//
//		if (cam_X > this->x_start || cam_X + screen_width < this->x_start)
//			SetRevival();
//	}
//}

//void CGoomba::SetRevival() {
//	this->SetPosition(x_start, y_start);
//	this->isDisable = false;
//	this->SetState(GOOMBA_STATE_WALKING);
//	y -= (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE);
//	die_start = NULL;
//}



void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += GOOMBA_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (y > 500) {
		this->SetState(GOOMBA_STATE_DIE);
	}

	if (this->state == GOOMBA_STATE_DIE && GetTickCount() - die_start > DIE_TIME) {
		this->isDisable = true;
	}

	//Revival();
	
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		float x0 = x;
		float y0 = y;

		x = x0 + dx;
		y = y0 + dy;

		if (!this->isDisable)
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

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (this->x <= this->lLimit) {
		this->x = this->lLimit;
		this->vx = -this->vx;
	}
	if (this->x >= this->rLimit - GOOMBA_BBOX_WIDTH) {
		this->x = this->rLimit - GOOMBA_BBOX_WIDTH;
		this->vx = -this->vx;
	}
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}

	animations[ani]->Render(x, y);
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE;
		vx = 0;
		die_start = GetTickCount();
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
	}
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;
}