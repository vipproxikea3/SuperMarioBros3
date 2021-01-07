#include "ParaKoopa.h"
#include "Block.h"
#include "Goomba.h"
#include "Mario.h"
#include "PlayScene.h"

void CParaKoopa::lvlDown() {
	if (this->GetLevel() == PARAKOOPA_LEVEL_WING) {
		this->SetLevel(PARAKOOPA_LEVEL_KOOPA);
	}
	else if (this->GetLevel() == PARAKOOPA_LEVEL_KOOPA) {
		level = PARAKOOPA_LEVEL_SHELL;
		this->SetState(PARAKOOPA_SHELL_STATE_IDLE);
	}
}

void CParaKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ReSet();
	if (!this->isDisable) {
		if ((this->GetState() == PARAKOOPA_SHELL_STATE_IDLE || this->GetState() == PARAKOOPA_SHELL_STATE_BEHUG) && GetTickCount64() - shell_start > PARAKOOPA_SHELL_TIME) {
			if (this->isHugging == true) {
				isHugging = false;
				CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
				mario->StopHug();
				mario->lvlDown();
				mario = NULL;
			}
			this->SetLevel(PARAKOOPA_LEVEL_WING);
			this->SetState(PARAKOOPA_STATE_WALKING);
			y -= PARAKOOPA_BBOX_HEIGHT - PARAKOOPA_SHELL_BBOX_HEIGHT;
		}		

		if (this->level == PARAKOOPA_LEVEL_KOOPA || this->level == PARAKOOPA_LEVEL_WING) {

			CGameObject::Update(dt, coObjects);
			vy += PARAKOOPA_GRAVITY * dt;

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
							this->vy = -PARAKOOPA_JUMP_SPEED;
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

			if (this->x <= this->limitL) {
				this->vx = PARAKOOPA_WALKING_SPEED;
			}
			if (this->x >= this->limitR - PARAKOOPA_BBOX_WIDTH) {
				this->vx = -PARAKOOPA_WALKING_SPEED;
			}
		}
		else {
			if (!isHugging) {
				// Calculate dx, dy 
				CGameObject::Update(dt);

				// Simple fall down
				vy += PARAKOOPA_SHELL_GRAVITY * dt;

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

						// BRICKREWARD
						/*if (dynamic_cast<CBrickReward*>(e->obj)) {
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
						}*/

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

void CParaKoopa::BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0)
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

void CParaKoopa::Render()
{
	switch (this->level)
	{
	case PARAKOOPA_LEVEL_WING:
		if (vx > 0) {
			animation_set->at(PARAKOOPA_ANI_WALKING_RIGHT)->Render(x, y, 255);
			if (vy < 0) {
				animation_set->at(PARAKOOPA_WING_RIGHT_ANI_JUMP)->Render(x, y - 2.0, 255);
			}
			else {
				animation_set->at(PARAKOOPA_WING_RIGHT_ANI_WALKING)->Render(x, y - 2.0, 255);
			}
		}
		else {
			animation_set->at(PARAKOOPA_ANI_WALKING_LEFT)->Render(x, y, 255);
			if (vy < 0) {
				animation_set->at(PARAKOOPA_WING_LEFT_ANI_JUMP)->Render(x + 8.0, y - 2.0, 255);
			}
			else {
				animation_set->at(PARAKOOPA_WING_LEFT_ANI_WALKING)->Render(x + 8.0, y - 2.0, 255);
			}
		}
		break;
	case PARAKOOPA_LEVEL_KOOPA:
		if (vx > 0) {
			animation_set->at(PARAKOOPA_ANI_WALKING_RIGHT)->Render(x, y, 255);
		}
		else {
			animation_set->at(PARAKOOPA_ANI_WALKING_LEFT)->Render(x, y, 255);
		}
		break;
	case PARAKOOPA_LEVEL_SHELL:
		switch (this->GetState()) {
		case PARAKOOPA_SHELL_STATE_IDLE:
			animation_set->at(PARAKOOPA_SHELL_ANI_IDLE)->Render(x, y, 255);
			break;
		case PARAKOOPA_SHELL_STATE_WALKING:
			animation_set->at(PARAKOOPA_SHELL_ANI_WALKING)->Render(x, y, 255);
			break;
		case PARAKOOPA_SHELL_STATE_BEHUG:
			animation_set->at(PARAKOOPA_SHELL_ANI_BEHUG)->Render(x, y, 255);
			break;
		}
		break;
	}
}

void CParaKoopa::ReSet() {
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);
	CGame* game = CGame::GetInstance();
	if (!game->IsInCamera(l, t, r, b)) {
		this->isDisable = true;
		this->isReadyReset = false;
		this->SetPosition(this->x_start, this->y_start);
		this->SetState(PARAKOOPA_STATE_WALKING);
		this->SetLevel(PARAKOOPA_LEVEL_WING);
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

void CParaKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PARAKOOPA_STATE_WALKING:
		isHugging = false;
		vx = -PARAKOOPA_WALKING_SPEED;
		vy = 0;
		nx = -1;
		break;
	case PARAKOOPA_SHELL_STATE_IDLE:
		isHugging = false;
		shell_start = GetTickCount64();
		vx = 0;
		vy = 0;
		break;
	case PARAKOOPA_SHELL_STATE_WALKING:
		isHugging = false;
		vy = 0;
		nx = 1;
		vx = PARAKOOPA_SHELL_WALKING_SPEED;
		break;
	case PARAKOOPA_SHELL_STATE_BEHUG:
		isHugging = true;
		vx = 0;
		vy = 0;
		break;
	}
}

void CParaKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (level == PARAKOOPA_LEVEL_SHELL) {
		bottom = top + PARAKOOPA_SHELL_BBOX_HEIGHT;
		right = left + PARAKOOPA_SHELL_BBOX_WIDTH;
	}
	else {
		bottom = top + PARAKOOPA_BBOX_HEIGHT;
		right = left + PARAKOOPA_BBOX_WIDTH;
	}
}