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
			this->SetLevel(PARAKOOPA_LEVEL_KOOPA);
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (mario->x < this->x) {
				this->SetState(PARAKOOPA_STATE_WALKING_LEFT);
			}
			else {
				this->SetState(PARAKOOPA_STATE_WALKING_RIGHT);
			}
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

					// BLOCK
					if (dynamic_cast<CBlock*>(e->obj)) {
						CBlock* block = dynamic_cast<CBlock*>(e->obj);

						if (e->nx == -1 && block->isBlockLeft()) {
							this->SetState(PARAKOOPA_STATE_WALKING_LEFT);
							this->x = x0 + min_tx * this->dx + nx * 0.4f;
						}

						if (e->nx == 1 && block->isBlockRight()) {
							this->SetState(PARAKOOPA_STATE_WALKING_RIGHT);
							this->x = x0 + min_tx * this->dx + nx * 0.4f;
						}

						if (e->ny == -1 && block->isBlockTop()) {
							if (this->GetLevel() == PARAKOOPA_LEVEL_WING)
								this->vy = -PARAKOOPA_JUMP_SPEED;
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

						if (e->nx == -1) {
							this->SetState(PARAKOOPA_STATE_WALKING_LEFT);
						}
						else if (e->nx == 1) {
							this->SetState(PARAKOOPA_STATE_WALKING_RIGHT);
						}

						if (e->ny == -1)
						{
							if (this->GetLevel() == PARAKOOPA_LEVEL_WING)
								this->vy = -PARAKOOPA_JUMP_SPEED;
						}
					}

					// BRICKREWARD
					if (dynamic_cast<CBrickReward*>(e->obj)) {
						BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
						if (e->nx != 0) {
							if (e->nx == -1)
								this->SetState(PARAKOOPA_STATE_WALKING_LEFT);
							else
								this->SetState(PARAKOOPA_STATE_WALKING_RIGHT);
						}

						if (e->ny == -1)
						{
							if (this->GetLevel() == PARAKOOPA_LEVEL_WING)
								this->vy = -PARAKOOPA_JUMP_SPEED;
						}
					}
				}
			}

			// clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
								this->SetState(SHELL_STATE_WALKING_LEFT);
								this->x = x0 + min_tx * this->dx + nx * 0.4f;
							}

							if (e->nx == 1 && block->isBlockRight()) {
								this->SetState(SHELL_STATE_WALKING_RIGHT);
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
							BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);

							if (e->nx == -1) {
								this->SetState(SHELL_STATE_WALKING_LEFT);
							}
							else if (e->nx == 1) {
								this->SetState(SHELL_STATE_WALKING_RIGHT);
							}

							CBreakBlock* breakBlock = dynamic_cast<CBreakBlock*>(e->obj);
							if (e->nx != 0 && breakBlock->GetState() == BREAKBLOCK_STATE_IDLE) {
								breakBlock->ShowPiece();
							}
						}

						// BRICKREWARD
						if (dynamic_cast<CBrickReward*>(e->obj)) {
							BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
							if (e->nx != 0) {
								if (e->nx == -1)
									this->SetState(SHELL_STATE_WALKING_LEFT);
								else
									this->SetState(SHELL_STATE_WALKING_RIGHT);
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
								ShowPoint();
							}
						}

						// PARAGOOMBA
						if (dynamic_cast<CParaGoomba*>(e->obj))
						{
							CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);
							if (paraGoomba->GetState() != PARAGOOMBA_STATE_DIE_X && paraGoomba->GetState() != PARAGOOMBA_STATE_DIE_Y)
							{
								paraGoomba->SetLevel(PARAGOOMBA_LEVEL_GOOMBA);
								paraGoomba->SetState(PARAGOOMBA_STATE_DIE_X);
								ShowPoint();
							}
						}

						//	KOOPA
						if (dynamic_cast<CKoopa*>(e->obj))
						{
							if (this->GetState() == SHELL_STATE_WALKING_LEFT || this->GetState() == SHELL_STATE_WALKING_RIGHT)
							{
								CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
								if (koopa->GetState() != KOOPA_STATE_DIE)
								{
									this->SetLevel(PARAKOOPA_LEVEL_SHELL);
									this->SetState(PARAKOOPA_STATE_DIE);
									koopa->SetLevel(KOOPA_LEVEL_SHELL);
									koopa->SetState(KOOPA_STATE_DIE);
									ShowPoint();
								}
							}
						}

						//	PARAKOOPA
						if (dynamic_cast<CParaKoopa*>(e->obj))
						{
							if (this->GetState() == SHELL_STATE_WALKING_LEFT || this->GetState() == SHELL_STATE_WALKING_RIGHT)
							{
								CParaKoopa* paraKoopa = dynamic_cast<CParaKoopa*>(e->obj);
								if (paraKoopa->GetState() != KOOPA_STATE_DIE)
								{
									this->SetLevel(PARAKOOPA_LEVEL_SHELL);
									this->SetState(PARAKOOPA_STATE_DIE);
									paraKoopa->SetLevel(PARAKOOPA_LEVEL_SHELL);
									paraKoopa->SetState(PARAKOOPA_STATE_DIE);
									ShowPoint();
								}
							}
						}
					}
				}

				// clean up collision events
				for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
			}
		}

		if (this->isDisable == false && (this->GetState() == PARAKOOPA_STATE_WALKING_LEFT || this->GetState() == PARAKOOPA_STATE_WALKING_RIGHT))
		{
			if (this->BeAttackByTail()) {
				this->SetLevel(PARAKOOPA_LEVEL_SHELL);
				this->SetState(PARAKOOPA_SHELL_STATE_OVERTURN);
				ShowPoint();
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
	if (this->type == PARAKOOPA_TYPE_GREEN)
	{
		switch (this->level)
		{
		case PARAKOOPA_LEVEL_WING:
			if (vx > 0) {
				animation_set->at(PARAKOOPA_GREEN_ANI_WALKING_RIGHT)->Render(x, y, 255);
				if (vy < 0) {
					animation_set->at(PARAKOOPA_WING_RIGHT_ANI_JUMP)->Render(x, y - 2.0, 255);
				}
				else {
					animation_set->at(PARAKOOPA_WING_RIGHT_ANI_WALKING)->Render(x, y - 2.0, 255);
				}
			}
			else {
				animation_set->at(PARAKOOPA_GREEN_ANI_WALKING_LEFT)->Render(x, y, 255);
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
				animation_set->at(PARAKOOPA_GREEN_ANI_WALKING_RIGHT)->Render(x, y, 255);
			}
			else {
				animation_set->at(PARAKOOPA_GREEN_ANI_WALKING_LEFT)->Render(x, y, 255);
			}
			break;
		case PARAKOOPA_LEVEL_SHELL:
			switch (this->GetState()) {
			case PARAKOOPA_SHELL_STATE_IDLE:
				animation_set->at(PARAKOOPA_GREEN_SHELL_ANI_IDLE)->Render(x, y, 255);
				break;
			case PARAKOOPA_SHELL_STATE_WALKING_LEFT:
				animation_set->at(PARAKOOPA_GREEN_SHELL_ANI_WALKING)->Render(x, y, 255);
				break;
			case PARAKOOPA_SHELL_STATE_WALKING_RIGHT:
				animation_set->at(PARAKOOPA_GREEN_SHELL_ANI_WALKING)->Render(x, y, 255);
				break;
			case PARAKOOPA_SHELL_STATE_BEHUG:
				animation_set->at(PARAKOOPA_GREEN_SHELL_ANI_BEHUG)->Render(x, y, 255);
				break;
			case PARAKOOPA_SHELL_STATE_OVERTURN:
				animation_set->at(PARAKOOPA_GREEN_SHELL_ANI_OVERTURN)->Render(x, y, 255);
				break;
			case PARAKOOPA_STATE_DIE:
				animation_set->at(PARAKOOPA_GREEN_ANI_DIE)->Render(x, y, 255);
				break;
			}
			break;
		}
	}
	else
	{
		switch (this->level)
		{
		case PARAKOOPA_LEVEL_WING:
			if (vx > 0) {
				animation_set->at(PARAKOOPA_RED_ANI_WALKING_RIGHT)->Render(x, y, 255);
				if (vy < 0) {
					animation_set->at(PARAKOOPA_WING_RIGHT_ANI_JUMP)->Render(x, y - 2.0, 255);
				}
				else {
					animation_set->at(PARAKOOPA_WING_RIGHT_ANI_WALKING)->Render(x, y - 2.0, 255);
				}
			}
			else {
				animation_set->at(PARAKOOPA_RED_ANI_WALKING_LEFT)->Render(x, y, 255);
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
				animation_set->at(PARAKOOPA_RED_ANI_WALKING_RIGHT)->Render(x, y, 255);
			}
			else {
				animation_set->at(PARAKOOPA_RED_ANI_WALKING_LEFT)->Render(x, y, 255);
			}
			break;
		case PARAKOOPA_LEVEL_SHELL:
			switch (this->GetState()) {
			case PARAKOOPA_SHELL_STATE_IDLE:
				animation_set->at(PARAKOOPA_RED_SHELL_ANI_IDLE)->Render(x, y, 255);
				break;
			case PARAKOOPA_SHELL_STATE_WALKING_LEFT:
				animation_set->at(PARAKOOPA_RED_SHELL_ANI_WALKING)->Render(x, y, 255);
				break;
			case PARAKOOPA_SHELL_STATE_WALKING_RIGHT:
				animation_set->at(PARAKOOPA_RED_SHELL_ANI_WALKING)->Render(x, y, 255);
				break;
			case PARAKOOPA_SHELL_STATE_BEHUG:
				animation_set->at(PARAKOOPA_RED_SHELL_ANI_BEHUG)->Render(x, y, 255);
				break;
			case PARAKOOPA_SHELL_STATE_OVERTURN:
				animation_set->at(PARAKOOPA_RED_SHELL_ANI_OVERTURN)->Render(x, y, 255);
				break;
			case PARAKOOPA_STATE_DIE:
				animation_set->at(PARAKOOPA_RED_ANI_DIE)->Render(x, y, 255);
				break;
			}
			break;
		}
	}
}

void CParaKoopa::ReSet() {
	if (this->GetState() != PARAKOOPA_STATE_DIE)
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
				this->SetState(PARAKOOPA_STATE_WALKING_LEFT);
			}
			else {
				this->SetState(PARAKOOPA_STATE_WALKING_RIGHT);
			}
			this->SetLevel(PARAKOOPA_LEVEL_WING);
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

void CParaKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PARAKOOPA_STATE_WALKING_LEFT:
		isHugging = false;
		vx = -PARAKOOPA_WALKING_SPEED;
		vy = 0;
		nx = -1;
		break;
	case PARAKOOPA_STATE_WALKING_RIGHT:
		isHugging = false;
		vx = PARAKOOPA_WALKING_SPEED;
		vy = 0;
		nx = -1;
		break;
	case PARAKOOPA_SHELL_STATE_IDLE:
		isHugging = false;
		shell_start = GetTickCount64();
		vx = 0;
		vy = 0;
		break;
	case PARAKOOPA_SHELL_STATE_WALKING_RIGHT:
		isHugging = false;
		vy = 0;
		nx = 1;
		vx = PARAKOOPA_SHELL_WALKING_SPEED;
		break;
	case PARAKOOPA_SHELL_STATE_WALKING_LEFT:
		isHugging = false;
		vy = 0;
		nx = 1;
		vx = -PARAKOOPA_SHELL_WALKING_SPEED;
		break;
	case PARAKOOPA_SHELL_STATE_BEHUG:
		isHugging = true;
		vx = 0;
		vy = 0;
		break;
	case PARAKOOPA_SHELL_STATE_OVERTURN:
		isHugging = false;
		vx = 0;
		vy = -PARAKOOPA_SHELL_JUMP_DEFLECT_SPEED;
		shell_start = GetTickCount64();
		break;
	case PARAKOOPA_STATE_DIE:
		isHugging = false;
		vy = -PARAKOOPA_DIE_DEFLECT_SPEED;
		break;
	}
}

void CParaKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (this->GetState() == PARAKOOPA_STATE_DIE) {
		bottom = top;
		right = left;
	}
	else
	{
		if (level == PARAKOOPA_LEVEL_SHELL) {
			bottom = top + PARAKOOPA_SHELL_BBOX_HEIGHT;
			right = left + PARAKOOPA_SHELL_BBOX_WIDTH;
		}
		else {
			bottom = top + PARAKOOPA_BBOX_HEIGHT;
			right = left + PARAKOOPA_BBOX_WIDTH;
		}
	}
}