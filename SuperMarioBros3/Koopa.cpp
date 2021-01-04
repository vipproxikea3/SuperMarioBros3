#include "Koopa.h"
#include "Block.h"
#include "Goomba.h"
#include "Mario.h"
#include "PlayScene.h"

void CKoopa::lvlDown() {
	level = KOOPA_LEVEL_SHELL;
	this->SetState(SHELL_STATE_IDLE);
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ReSet();
	if (!this->isDisable) {
		if ((this->GetState() == SHELL_STATE_IDLE || this->GetState() == SHELL_STATE_BEHUG) && GetTickCount64() - shell_start > SHELL_TIME) {	
			if (this->isHugging == true) {
				isHugging = false;
				CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
				mario->StopHug();
				mario->lvlDown();
				mario = NULL;
			}
			this->SetLevel(KOOPA_LEVEL_KOOPA);
			this->SetState(KOOPA_STATE_WALKING);
			y -= KOOPA_BBOX_HEIGHT - SHELL_BBOX_HEIGHT;
		}

		if (this->level == KOOPA_LEVEL_KOOPA) {
			CGameObject::Update(dt, coObjects);
			vy += KOOPA_GRAVITY * dt;

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

					// BREAKBLOCK
					if (dynamic_cast<CBreakBlock*>(e->obj)) {
						CBreakBlock* breakBlock = dynamic_cast<CBreakBlock*>(e->obj);
						BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					}
				}
			}

			// clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

			if (this->x <= this->limitL) {
				//this->x = this->limitL;
				this->vx = KOOPA_WALKING_SPEED;
			}
			if (this->x >= this->limitR - KOOPA_BBOX_WIDTH) {
				//this->x = this->limitR - KOOPA_BBOX_WIDTH;
				this->vx = -KOOPA_WALKING_SPEED;
			}
		}
		else {
			if (!isHugging) {
				// Calculate dx, dy 
				CGameObject::Update(dt);

				// Simple fall down
				vy += SHELL_GRAVITY * dt;

				vector<LPCOLLISIONEVENT> coEvents;
				vector<LPCOLLISIONEVENT> coEventsResult;

				coEvents.clear();

				CalcPotentialCollisions(coObjects, coEvents);

				// No collision occured, proceed normally
				if (coEvents.size() == 0)
				{
					x += dx;
					y += dy;
				}
				else
				{
					float min_tx, min_ty, nx = 0, ny;
					float rdx = 0;
					float rdy = 0;

					FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

					float x0 = x;
					float y0 = y;


					x = x0 + dx;
					y = y0 + dy;

					for (UINT i = 0; i < coEventsResult.size(); i++)
					{
						LPCOLLISIONEVENT e = coEventsResult[i];

						// BLOCK
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

						// BREAKBLOCK
						if (dynamic_cast<CBreakBlock*>(e->obj)) {
							CBreakBlock* breakBlock = dynamic_cast<CBreakBlock*>(e->obj);
							BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);

							if (e->nx == 1) {
								this->vx = SHELL_WALKING_SPEED;
								breakBlock->isDisable = true;
							}
							else if (e->nx == -1) {
								this->vx = -SHELL_WALKING_SPEED;
								breakBlock->isDisable = true;
							}
						}

						// BRICKREWARD
						if (dynamic_cast<CBrickReward*>(e->obj)) {
							BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
							if (e->nx != 0) {
								if (e->nx == -1)
									vx = -SHELL_WALKING_SPEED;
								else
									vx = SHELL_WALKING_SPEED;
							}


							CBrickReward* brick = dynamic_cast<CBrickReward*>(e->obj);
							if (e->nx != 0 && brick->GetState() == BRICKREWARD_STATE_IDLE) {
								brick->SetState(BRICKREWARD_STATE_JUMP);
							}
						}

						// GOOMBA
						if (dynamic_cast<CGoomba*>(e->obj))
						{
							CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
							if (goomba->GetState() != GOOMBA_STATE_DIE_X) {
								goomba->SetState(GOOMBA_STATE_DIE_X);
							}
						}

						// PARAGOOMBA
						if (dynamic_cast<CParaGoomba*>(e->obj))
						{
							CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);
							if (paraGoomba->GetLevel() == PARAGOOMBA_LEVEL_WING) {
								paraGoomba->SetLevel(PARAGOOMBA_LEVEL_GOOMBA);
							}
							else {
								paraGoomba->SetState(PARAGOOMBA_STATE_DIE_X);
							}
						}
					}
				}

				// clean up collision events
				for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
			}
		}
	}
}

void CKoopa::BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0)
{
	if (nx != 0)
	{
		this->vx = 0;
		this->x = x0 + min_tx * this->dx + nx * 0.1f;
	}
	if (ny != 0)
	{
		this->vy = 0;
		this->y = y0 + min_ty * this->dy + ny * 0.1f;
	}
}

void CKoopa::Render()
{
	int ani;
	if (this->level == KOOPA_LEVEL_KOOPA) {
		if (vx > 0)
			ani = KOOPA_ANI_WALKING_RIGHT;
		else
			ani = KOOPA_ANI_WALKING_LEFT;
	}
	else {
		switch (this->GetState()) {
		case SHELL_STATE_IDLE:
			ani = SHELL_ANI_IDLE;
			break;
		case SHELL_STATE_WALKING:
			ani = SHELL_ANI_WALKING;
			break;
		case SHELL_STATE_BEHUG:
			ani = SHELL_ANI_BEHUG;
			break;
		}
	}

	animation_set->at(ani)->Render(x, y, 255);
}

void CKoopa::ReSet() {
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);
	CGame* game = CGame::GetInstance();
	if (!game->IsInCamera(l, t, r, b)) {
		this->isDisable = true;
		this->isReadyReset = false;
		this->SetPosition(this->x_start, this->y_start);
		this->SetState(KOOPA_STATE_WALKING);
		this->SetLevel(KOOPA_LEVEL_KOOPA);
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

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_WALKING:
		isHugging = false;
		vx = -KOOPA_WALKING_SPEED;
		vy = 0;
		nx = -1;
		break;
	case SHELL_STATE_IDLE:
		isHugging = false;
		shell_start = GetTickCount64();
		vx = 0;
		vy = 0;
		break;
	case SHELL_STATE_WALKING:
		isHugging = false;
		vy = 0;
		nx = 1;
		vx = SHELL_WALKING_SPEED;
		break;
	case SHELL_STATE_BEHUG:
		isHugging = true;
		vx = 0;
		vy = 0;
		break;
	}
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	
	if (level == KOOPA_LEVEL_SHELL) {
		bottom = top + SHELL_BBOX_HEIGHT;
		right = left + SHELL_BBOX_WIDTH;
	}
	else {
		bottom = top + KOOPA_BBOX_HEIGHT;
		right = left + KOOPA_BBOX_WIDTH;
	}
}