#include "Koopa.h"
#include "Block.h"
#include "Goomba.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Point.h"
#include "Utils.h"

void CKoopa::lvlDown() {
	level = KOOPA_LEVEL_SHELL;
	this->SetState(SHELL_STATE_IDLE);
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ReSet();
	if (!this->isDisable) {

		if ((this->GetState() == SHELL_STATE_IDLE || this->GetState() == SHELL_STATE_BEHUG || this->GetState() == SHELL_STATE_OVERTURN) && GetTickCount64() - shell_start > SHELL_TIME) {
			if (this->isHugging == true) {
				isHugging = false;
				CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
				mario->StopHug();
				mario->lvlDown();
				mario = NULL;
			}
			this->SetLevel(KOOPA_LEVEL_KOOPA);
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (mario->x < this->x) {
				this->SetState(KOOPA_STATE_WALKING_LEFT);
			}
			else {
				this->SetState(KOOPA_STATE_WALKING_RIGHT);
			}
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
							this->SetState(KOOPA_STATE_WALKING_LEFT);
							this->x = x0 + min_tx * this->dx + nx * 0.4f;
						}

						if (e->nx == 1 && block->isBlockRight()) {
							this->SetState(KOOPA_STATE_WALKING_RIGHT);
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

					// BRICK REWARD
					if (dynamic_cast<CBrickReward*>(e->obj)) {
						CBrickReward* brick = dynamic_cast<CBrickReward*>(e->obj);
						BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
						if (e->nx != 0) {
							if (this->GetState() == KOOPA_STATE_WALKING_LEFT)
								this->SetState(KOOPA_STATE_WALKING_RIGHT);
							else if (this->GetState() == KOOPA_STATE_WALKING_RIGHT)
								this->SetState(KOOPA_STATE_WALKING_LEFT);
						}
					}

					// BREAK BLOCK
					if (dynamic_cast<CBreakBlock*>(e->obj)) {
						CBreakBlock* brick = dynamic_cast<CBreakBlock*>(e->obj);
						BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
						if (e->nx == 1.0) {
							this->SetState(KOOPA_STATE_WALKING_RIGHT);
						}
						if (e->nx == -1.0) {
							this->SetState(KOOPA_STATE_WALKING_LEFT);
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
				vy += SHELL_GRAVITY * dt;

				vector<LPCOLLISIONEVENT> coEvents;
				vector<LPCOLLISIONEVENT> coEventsResult;

				coEvents.clear();

				CalcPotentialCollisions(coObjects, coEvents);

				// No collision occured, proceed normally
				if (this->GetState() == KOOPA_STATE_DIE)
				{
					x += dx;
					y += dy;
				}
				else if (coEvents.size() == 0)
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
									this->SetLevel(KOOPA_LEVEL_SHELL);
									this->SetState(KOOPA_STATE_DIE);
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
									this->SetLevel(KOOPA_LEVEL_SHELL);
									this->SetState(KOOPA_STATE_DIE);
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

		if (CalculateTurningAround(coObjects))
			TurnAround();

		if (this->isDisable == false && (this->GetState() == KOOPA_STATE_WALKING_LEFT || this->GetState() == KOOPA_STATE_WALKING_RIGHT))
		{
			if (this->BeAttackByTail()) {
				this->SetLevel(KOOPA_LEVEL_SHELL);
				this->SetState(SHELL_STATE_OVERTURN);
				ShowPoint();
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

bool CKoopa::CalculateTurningAround(vector<LPGAMEOBJECT>* coObjects)
{
	bool onGround = false;
	if (type != KOOPA_TYPE_RED)
		return false;
	if (this->GetState() != KOOPA_STATE_WALKING_LEFT && this->GetState() != KOOPA_STATE_WALKING_RIGHT)
		return false;
	if (this->level != KOOPA_LEVEL_KOOPA)
		return false;
	float kl, kt, kr, kb;
	GetBoundingBox(kl, kt, kr, kb);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT object = coObjects->at(i);
		if (!object->IsInCamera())
			continue;

		if (!dynamic_cast<CBlock*>(object) && !dynamic_cast<CBrickReward*>(object) && !dynamic_cast<CBreakBlock*>(object))
			continue;

		//object is Break or RBox
		float ol, ot, or , ob;
		object->GetBoundingBox(ol, ot, or , ob);

		if (kb > ot - 2 && kb < ot && ((kl >= ol && kl <= or) || (kr >= ol && kr <= or)))
			onGround = true;

		if (kb > ot - 2 && kb < ot)
		{
			if (state == KOOPA_STATE_WALKING_LEFT)
			{
				if ((kl + 4 > ol && kl + 8 < or ) || (kl + 4 <= or && kl + 8 >= or))
					return false;
			}
			else if (state == KOOPA_STATE_WALKING_RIGHT)
			{
				if ((kr - 8 > ol && kr - 4 < or ) || (kr - 8 <= ol && kr - 4 >= ol))
					return false;
			}
		}

	}
	if (!onGround) return false;
	return true;
}

void CKoopa::TurnAround()
{
	if (this->GetState() == KOOPA_STATE_WALKING_LEFT)
		this->SetState(KOOPA_STATE_WALKING_RIGHT);
	else if (this->GetState() == KOOPA_STATE_WALKING_RIGHT)
		this->SetState(KOOPA_STATE_WALKING_LEFT);
}

void CKoopa::Render()
{
	int ani;
	if (this->type == KOOPA_TYPE_GREEN) {
		if (this->level == KOOPA_LEVEL_KOOPA) {
			if (vx > 0)
				ani = KOOPA_GREEN_ANI_WALKING_RIGHT;
			else
				ani = KOOPA_GREEN_ANI_WALKING_LEFT;
		}
		else {
			switch (this->GetState()) {
			case SHELL_STATE_IDLE:
				ani = SHELL_GREEN_ANI_IDLE;
				break;
			case SHELL_STATE_WALKING_LEFT:
				ani = SHELL_GREEN_ANI_WALKING;
				break;
			case SHELL_STATE_WALKING_RIGHT:
				ani = SHELL_GREEN_ANI_WALKING;
				break;
			case SHELL_STATE_BEHUG:
				ani = SHELL_GREEN_ANI_BEHUG;
				break;
			case SHELL_STATE_OVERTURN:
				ani = SHELL_GREEN_ANI_OVERTURN;
				break;
			case KOOPA_STATE_DIE:
				ani = KOOPA_GREEN_ANI_DIE;
				break;
			}
		}
	}
	else
	{
		if (this->level == KOOPA_LEVEL_KOOPA) {
			if (vx > 0)
				ani = KOOPA_RED_ANI_WALKING_RIGHT;
			else
				ani = KOOPA_RED_ANI_WALKING_LEFT;
		}
		else {
			switch (this->GetState()) {
			case SHELL_STATE_IDLE:
				ani = SHELL_RED_ANI_IDLE;
				break;
			case SHELL_STATE_WALKING_LEFT:
				ani = SHELL_RED_ANI_WALKING;
				break;
			case SHELL_STATE_WALKING_RIGHT:
				ani = SHELL_RED_ANI_WALKING;
				break;
			case SHELL_STATE_BEHUG:
				ani = SHELL_RED_ANI_BEHUG;
				break;
			case SHELL_STATE_OVERTURN:
				ani = SHELL_RED_ANI_OVERTURN;
				break;
			case KOOPA_STATE_DIE:
				ani = KOOPA_RED_ANI_DIE;
				break;
			}
		}
	}
	
	animation_set->at(ani)->Render(x, y, 255);
}

void CKoopa::ReSet() {
	if (this->GetState() != KOOPA_STATE_DIE)
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
				this->SetState(KOOPA_STATE_WALKING_LEFT);
			}
			else {
				this->SetState(KOOPA_STATE_WALKING_RIGHT);
			}
			this->SetLevel(KOOPA_LEVEL_KOOPA);
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

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_WALKING_LEFT:
		isHugging = false;
		vx = -KOOPA_WALKING_SPEED;
		vy = 0;
		nx = -1;
		break;
	case KOOPA_STATE_WALKING_RIGHT:
		isHugging = false;
		vx = KOOPA_WALKING_SPEED;
		vy = 0;
		nx = 1;
		break;
	case SHELL_STATE_IDLE:
		isHugging = false;
		shell_start = GetTickCount64();
		vx = 0;
		vy = 0;
		break;
	case SHELL_STATE_WALKING_LEFT:
		isHugging = false;
		vy = 0;
		nx = -1;
		vx = -SHELL_WALKING_SPEED;
		break;
	case SHELL_STATE_WALKING_RIGHT:
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
	case SHELL_STATE_OVERTURN:
		isHugging = false;
		vx = 0;
		vy = -SHELL_JUMP_DEFLECT_SPEED;
		shell_start = GetTickCount64();
		break;
	case KOOPA_STATE_DIE:
		isHugging = false;
		vy = -KOOPA_DIE_DEFLECT_SPEED;
		break;
	}

	DebugOut(L"[STATE] %d\n", state);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (this->GetState() == KOOPA_STATE_DIE) {
		bottom = top;
		right = left;
	}
	else {
		if (level == KOOPA_LEVEL_SHELL) {
			bottom = top + SHELL_BBOX_HEIGHT;
			right = left + SHELL_BBOX_WIDTH;
		}
		else {
			bottom = top + KOOPA_BBOX_HEIGHT;
			right = left + KOOPA_BBOX_WIDTH;
		}
	}
}