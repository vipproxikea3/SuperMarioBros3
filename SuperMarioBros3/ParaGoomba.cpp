#include "ParaGoomba.h"
#include "Block.h"
#include "Game.h"
#include "BrickReward.h"
#include "BreakBlock.h"
#include "PlayScene.h"

CParaGoomba::CParaGoomba() {
	this->SetState(PARAGOOMBA_STATE_WALKING_LEFT);
	this->SetLevel(PARAGOOMBA_LEVEL_WING);
	last_jump = GetTickCount64();
}

CParaGoomba::~CParaGoomba() {}

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ReSet();
	if (!this->isDisable) {

		if (GetTickCount64() - last_jump > PARAGOOMBA_JUMP_COOLDOWN && this->GetLevel() == PARAGOOMBA_LEVEL_WING) {
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (mario->x < this->x) {
				this->SetState(PARAGOOMBA_STATE_WALKING_LEFT);
			}
			else {
				this->SetState(PARAGOOMBA_STATE_WALKING_RIGHT);
			}
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

				if (dynamic_cast<CBrickReward*>(e->obj)) {
					CBrickReward* brick = dynamic_cast<CBrickReward*>(e->obj);
					BasicCollision(min_tx, min_ty, nx, ny, x0, y0);
					if (e->nx != 0) {
						if (this->GetState() == PARAGOOMBA_STATE_WALKING_LEFT)
							this->SetState(PARAGOOMBA_STATE_WALKING_RIGHT);
						else if (this->GetState() == PARAGOOMBA_STATE_WALKING_RIGHT)
							this->SetState(PARAGOOMBA_STATE_WALKING_LEFT);
					}
				}

				if (dynamic_cast<CBreakBlock*>(e->obj)) {
					CBreakBlock* brick = dynamic_cast<CBreakBlock*>(e->obj);
					BasicCollision(min_tx, min_ty, nx, ny, x0, y0);
					if (e->nx != 0) {
						if (this->GetState() == PARAGOOMBA_STATE_WALKING_LEFT)
							this->SetState(PARAGOOMBA_STATE_WALKING_RIGHT);
						else if (this->GetState() == PARAGOOMBA_STATE_WALKING_RIGHT)
							this->SetState(PARAGOOMBA_STATE_WALKING_LEFT);
					}
				}
			}
		}

		if (this->isDisable == false && this->GetState() != PARAGOOMBA_STATE_DIE_X && this->GetState() != PARAGOOMBA_STATE_DIE_Y)
		{
			if (this->BeAttackByTail()) {
				this->SetLevel(PARAGOOMBA_LEVEL_GOOMBA);
				this->SetState(PARAGOOMBA_STATE_DIE_X);
				ShowPoint();
				ShowTailAttackEffect();
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CParaGoomba::BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0)
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
	if (this->GetState() != PARAGOOMBA_STATE_DIE_X && this->GetState() != PARAGOOMBA_STATE_DIE_Y)
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
				this->SetState(PARAGOOMBA_STATE_WALKING_LEFT);
			}
			else {
				this->SetState(PARAGOOMBA_STATE_WALKING_RIGHT);
			}
			this->SetLevel(PARAGOOMBA_LEVEL_WING);
			last_jump = GetTickCount64();
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
	case PARAGOOMBA_STATE_WALKING_RIGHT:
		die_start = NULL;
		vx = PARAGOOMBA_WALKING_SPEED;
		vy = 0;
		break;
	case PARAGOOMBA_STATE_WALKING_LEFT:
		die_start = NULL;
		vx = -PARAGOOMBA_WALKING_SPEED;
		vy = 0;
		break;
	}
}

void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (this->GetState())
	{
	case PARAGOOMBA_STATE_WALKING_RIGHT:
		left = x;
		top = y;
		right = x + PARAGOOMBA_BBOX_WIDTH;
		bottom = y + PARAGOOMBA_BBOX_HEIGHT;
		break;
	case PARAGOOMBA_STATE_WALKING_LEFT:
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