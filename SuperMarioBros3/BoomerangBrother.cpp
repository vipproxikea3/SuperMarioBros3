#include "BoomerangBrother.h"
#include "PlayScene.h"

void CBoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	ReSet();
	if (!this->isDisable) {
		// Calculate dx, dy 
		CGameObject::Update(dt);

		// Simple fall down
		vy += BOOMERANGBROTHER_GRAVITY * dt;

		switch (state)
		{
		case BOOMERANGBROTHER_STATE_READY_LEFT:
			if (GetTickCount64() - last_shot > BOOMERANGBROTHER_TIME_SHOT && !shoted)
				Shot();
			break;
		case BOOMERANGBROTHER_STATE_READY_RIGHT:
			if (GetTickCount64() - last_shot > BOOMERANGBROTHER_TIME_SHOT && !shoted)
				Shot();
			break;
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

				// BLOCK
				if (dynamic_cast<CBlock*>(e->obj)) {
					CBlock* block = dynamic_cast<CBlock*>(e->obj);

					if (e->nx == -1 && block->isBlockLeft()) {
						vx = -vx;
						this->x = x0 + min_tx * this->dx + nx * 0.4f;
					}

					if (e->nx == 1 && block->isBlockRight()) {
						vx = -vx;
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

				// CBrick Reward
				if (dynamic_cast<CBrickReward*>(e->obj)) {
					CBrickReward* brick = dynamic_cast<CBrickReward*>(e->obj);
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					if (e->nx != 0) {
						vx = -vx;
					}
				}

				// Break Block
				if (dynamic_cast<CBreakBlock*>(e->obj)) {
					CBreakBlock* brick = dynamic_cast<CBreakBlock*>(e->obj);
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					if (e->nx != 0) {
						vx = -vx;
					}
				}
			}
		}

		// Check out zone
		if (x < x_start - BOOMERANGBROTHER_WALKING_ZONE) {
			x = x_start - BOOMERANGBROTHER_WALKING_ZONE;
			vx = -vx;
		}
		if (x > x_start + BOOMERANGBROTHER_WALKING_ZONE) {
			x = x_start + BOOMERANGBROTHER_WALKING_ZONE;
			vx = -vx;
		}

		SetSide();

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CBoomerangBrother::SetSide() {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->x < this->x) {
		if (state == BOOMERANGBROTHER_STATE_READY_RIGHT)
			this->SetState(BOOMERANGBROTHER_STATE_READY_LEFT);
		if (state == BOOMERANGBROTHER_STATE_ATTACK_RIGHT)
			this->SetState(BOOMERANGBROTHER_STATE_ATTACK_LEFT);
	}
	else {
		if (state == BOOMERANGBROTHER_STATE_READY_LEFT)
			this->SetState(BOOMERANGBROTHER_STATE_READY_RIGHT);
		if (state == BOOMERANGBROTHER_STATE_ATTACK_LEFT)
			this->SetState(BOOMERANGBROTHER_STATE_ATTACK_RIGHT);
	}
}

void CBoomerangBrother::Shot() {
	shoted = true;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(31);
	CBoomerang* boomerang = new CBoomerang();
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario)
	{
		boomerang->SetPosition(x, y - 8.0f);
		boomerang->SetDefaultPosition(x, y - 8.0f);
	}
	boomerang->SetAnimationSet(ani_set);

	if (state == BOOMERANGBROTHER_STATE_READY_LEFT) {
		boomerang->SetState(BOOMERANG_STATE_WALKING_LEFT_PHARSE_1);
		this->SetState(BOOMERANGBROTHER_STATE_ATTACK_LEFT);
	}

	if (state == BOOMERANGBROTHER_STATE_READY_RIGHT) {
		boomerang->SetState(BOOMERANG_STATE_WALKING_RIGHT_PHARSE_1);
		this->SetState(BOOMERANGBROTHER_STATE_ATTACK_RIGHT);
	}

	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(boomerang);
}

void CBoomerangBrother::ReSet()
{
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);
	CGame* game = CGame::GetInstance();
	if (!this->IsInCamera()) {
		this->isDisable = true;
		this->isReadyReset = false;
		this->SetPosition(this->x_start, this->y_start);
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario->x < this->x) {
			this->SetState(BOOMERANGBROTHER_STATE_READY_LEFT);
		}
		else {
			this->SetState(BOOMERANGBROTHER_STATE_READY_RIGHT);
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

void CBoomerangBrother::BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0)
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

void CBoomerangBrother::Render()
{
	int ani;
	int alpha = 255;

	switch (state)
	{
	case BOOMERANGBROTHER_STATE_READY_LEFT:
		animation_set->at(BOOMERANGBROTHER_ANI_READY_LEFT)->Render(x, y, alpha);
		animation_set->at(BOOMERANGBROTHER_ANI_BOOMERANG_LEFT)->Render(x + 8.0f, y - 8.0f, alpha);
		break;
	case BOOMERANGBROTHER_STATE_ATTACK_LEFT:
		animation_set->at(BOOMERANGBROTHER_ANI_ATTACK_LEFT)->Render(x, y, alpha);
		break;
	case BOOMERANGBROTHER_STATE_READY_RIGHT:
		animation_set->at(BOOMERANGBROTHER_ANI_READY_RIGHT)->Render(x, y, alpha);
		animation_set->at(BOOMERANGBROTHER_ANI_BOOMERANG_RIGHT)->Render(x - 8.0f, y - 8.0f, alpha);
		break;
	case BOOMERANGBROTHER_STATE_ATTACK_RIGHT:
		animation_set->at(BOOMERANGBROTHER_ANI_ATTACK_RIGHT)->Render(x, y, alpha);
		break;
	}
}

void CBoomerangBrother::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BOOMERANGBROTHER_STATE_READY_LEFT:
		last_shot = GetTickCount64();
		shoted = false;
		break;
	case BOOMERANGBROTHER_STATE_READY_RIGHT:
		last_shot = GetTickCount64();
		shoted = false;
		break;
	}
}

void CBoomerangBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANGBROTHER_BBOX_WIDTH;
	bottom = y + BOOMERANGBROTHER_BBOX_HEIGHT;
}