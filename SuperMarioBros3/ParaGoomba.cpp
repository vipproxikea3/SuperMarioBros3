#include "ParaGoomba.h"
#include "Block.h"
#include "Game.h"

CParaGoomba::CParaGoomba(float l, float r) {
	this->SetState(PARAGOOMBA_STATE_WALKING);
	this->SetLevel(PARAGOOMBA_LEVEL_WING);
	last_jump = GetTickCount64();
	this->SetActiveArea(l, r);
}

CParaGoomba::~CParaGoomba() {}

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ReSet();
	if (!this->isDisable) {

		if (GetTickCount64() - last_jump > PARAGOOMBA_JUMP_COOLDOWN && this->GetLevel() == PARAGOOMBA_LEVEL_WING) {
			vy = -PARAGOOMBA_JUMP_SPEED;
			last_jump = GetTickCount64();
		}

		CGameObject::Update(dt, coObjects);
		vy += PARAGOOMBA_GRAVITY * dt;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		if ((this->state == PARAGOOMBA_STATE_DIE_Y || this->state == PARAGOOMBA_STATE_DIE_X) && GetTickCount64() - die_start > PARAGOOMBA_DIE_TIME) {
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
		if (this->x >= this->rLimit - PARAGOOMBA_BBOX_WIDTH) {
			this->x = this->rLimit - PARAGOOMBA_BBOX_WIDTH;
			this->vx = -this->vx;
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CParaGoomba::Render()
{
	if (this->GetState() == PARAGOOMBA_STATE_DIE_X || this->GetState() == PARAGOOMBA_STATE_DIE_Y) {
		if (this->GetState() == PARAGOOMBA_STATE_DIE_X) {
			animation_set->at(PARAGOOMBA_ANI_DIE_X)->Render(x, y, 255);
		}
		else {
			animation_set->at(PARAGOOMBA_ANI_DIE_Y)->Render(x, y, 255);
		}
	}
	else {
		animation_set->at(PARAGOOMBA_ANI_WALKING)->Render(x, y, 255);
		if (this->vy < 0) {
			animation_set->at(PARAGOOMBA_WING_LEFT_ANI_JUMP)->Render(round(x - 4.0), round(y - 7.0), 255);
			animation_set->at(PARAGOOMBA_WING_RIGHT_ANI_JUMP)->Render(round(x + 12.0), round(y - 7.0), 255);
		}
		else {
			if (this->GetLevel() == PARAGOOMBA_LEVEL_WING) {
				animation_set->at(PARAGOOMBA_WING_LEFT_ANI_WALKING)->Render(round(x - 4.0), round(y - 7.0), 255);
				animation_set->at(PARAGOOMBA_WING_RIGHT_ANI_WALKING)->Render(round(x + 12.0), round(y - 7.0), 255);
			}
		}
	}
}

void CParaGoomba::ReSet() {
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);
	CGame* game = CGame::GetInstance();
	if (!game->IsInCamera(l, t, r, b)) {
		this->isDisable = true;
		this->isReadyReset = false;
		this->SetPosition(this->x_start, this->y_start);
		this->SetState(PARAGOOMBA_STATE_WALKING);
		this->SetLevel(PARAGOOMBA_LEVEL_WING);
		last_jump = GetTickCount64();
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

void CParaGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PARAGOOMBA_STATE_DIE_X:
		vy = -PARAGOOMBA_DIE_DEFLECT_SPEED;
		die_start = GetTickCount64();
		break;
	case PARAGOOMBA_STATE_DIE_Y:
		y += PARAGOOMBA_BBOX_HEIGHT - PARAGOOMBA_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
		die_start = GetTickCount64();
		break;
	case PARAGOOMBA_STATE_WALKING:
		die_start = NULL;
		vx = -PARAGOOMBA_WALKING_SPEED;
		vy = 0;
	}
}

void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (this->GetState())
	{
	case PARAGOOMBA_STATE_WALKING:
		left = x;
		top = y;
		right = x + PARAGOOMBA_BBOX_WIDTH;
		bottom = y + PARAGOOMBA_BBOX_HEIGHT;
		break;
	default:
		top = y;
		left = x;
		bottom = y;
		right = x;
		break;
	}
}