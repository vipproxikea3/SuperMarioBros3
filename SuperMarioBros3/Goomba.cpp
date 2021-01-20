#include "Goomba.h"
#include "Block.h"
#include "Game.h"
#include "BrickReward.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Utils.h"

CGoomba::CGoomba() {
	this->SetState(GOOMBA_STATE_WALKING_LEFT);
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
						this->SetState(GOOMBA_STATE_WALKING_LEFT);
						this->x = x0 + min_tx * this->dx + nx * 0.4f;
					}

					if (e->nx == 1 && block->isBlockRight()) {
						this->SetState(GOOMBA_STATE_WALKING_RIGHT);
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

				if (dynamic_cast<CBrickReward*>(e->obj)) {
					CBrickReward* brick = dynamic_cast<CBrickReward*>(e->obj);
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					if (e->nx != 0) {
						if (this->GetState() == GOOMBA_STATE_WALKING_LEFT)
							this->SetState(GOOMBA_STATE_WALKING_RIGHT);
						else if (this->GetState() == GOOMBA_STATE_WALKING_RIGHT)
							this->SetState(GOOMBA_STATE_WALKING_LEFT);
					}
				}

				if (dynamic_cast<CBreakBlock*>(e->obj)) {
					CBreakBlock* brick = dynamic_cast<CBreakBlock*>(e->obj);
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					if (e->nx != 0) {
						if (this->GetState() == GOOMBA_STATE_WALKING_LEFT)
							this->SetState(GOOMBA_STATE_WALKING_RIGHT);
						else if (this->GetState() == GOOMBA_STATE_WALKING_RIGHT)
							this->SetState(GOOMBA_STATE_WALKING_LEFT);
					}
				}

				if (dynamic_cast<CGoomba*>(e->obj)) {
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					if (e->nx != 0) {
						if (this->GetState() == GOOMBA_STATE_WALKING_LEFT)
							this->SetState(GOOMBA_STATE_WALKING_RIGHT);
						else if (this->GetState() == GOOMBA_STATE_WALKING_RIGHT)
							this->SetState(GOOMBA_STATE_WALKING_LEFT);

						if (goomba->GetState() == GOOMBA_STATE_WALKING_LEFT)
							goomba->SetState(GOOMBA_STATE_WALKING_RIGHT);
						else if (goomba->GetState() == GOOMBA_STATE_WALKING_RIGHT)
							goomba->SetState(GOOMBA_STATE_WALKING_LEFT);
					}
				}
			}
		}

		if (this->isDisable == false && this->GetState() != GOOMBA_STATE_DIE_X && this->GetState() != GOOMBA_STATE_DIE_Y)
		{
			if (this->BeAttackByTail()) {
				this->SetState(GOOMBA_STATE_DIE_X);
				ShowPoint();
				ShowTailAttackEffect();
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CGoomba::BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0)
{
	if (nx != 0)
	{
		this->vx = 0;
		this->x = x0 + min_tx * this->dx + nx * 0.4f;
	}
	if (ny != 0)
	{
		this->vy = 0;
		this->y = y0 + min_ty * this->dy + ny * 0.4f;
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
	if (this->GetState() != GOOMBA_STATE_DIE_X && this->GetState() != GOOMBA_STATE_DIE_Y) {
		float l, t, r, b;
		this->GetBoundingBox(l, t, r, b);
		CGame* game = CGame::GetInstance();
		if (!this->IsInCamera()) {
			this->isDisable = true;
			this->isReadyReset = false;
			this->SetPosition(this->x_start, this->y_start);
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (mario->x < this->x) {
				this->SetState(GOOMBA_STATE_WALKING_LEFT);
			}
			else {
				this->SetState(GOOMBA_STATE_WALKING_RIGHT);
			}
		}
		if (!game->IsInCamera(this->x_start, this->y_start, this->x_start + r - l, this->y_start + b - t)) {
			if (this->isDisable) {
				this->isReadyReset = true;
			}
		}
		if (this->IsInCamera()) {
			if (this->isReadyReset)
				this->isDisable = false;
		}
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
	case GOOMBA_STATE_WALKING_RIGHT:
		die_start = NULL;
		vx = GOOMBA_WALKING_SPEED;
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING_LEFT:
		die_start = NULL;
		vx = -GOOMBA_WALKING_SPEED;
		vy = 0;
		break;
	}
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (this->GetState())
	{
	case GOOMBA_STATE_WALKING_LEFT:
		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;
		bottom = y + GOOMBA_BBOX_HEIGHT;
		break;
	case GOOMBA_STATE_WALKING_RIGHT:
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