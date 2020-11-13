#include "Shell.h"
#include "Game.h"
#include "Block.h"
#include "BrickReward.h"
#include "Goomba.h"

void CShell::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (!isHugging)
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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		float x0 = x;
		float y0 = y;


		x = x0 + dx;
		y = y0 + dy;

		if (!this->isDisable && !this->isHugging)
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
					if (goomba->GetState() != GOOMBA_STATE_DIE) {
						goomba->SetState(GOOMBA_STATE_DIE);
					}
				}
			}
	}

	CGame* game = CGame::GetInstance();
	float cx, cy;
	game->GetCamPos(cx, cy);
	float screenWidth = game->GetScreenWidth();

	if (this->x < cx || this->x > cx + screenWidth) {
		this->isDisable = true;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CShell::BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0)
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

void CShell::Render()
{
	int ani;

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

	int alpha = 255;
	animations[ani]->Render(x, y, alpha);
}

void CShell::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SHELL_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case SHELL_STATE_WALKING:
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

void CShell::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SHELL_SMALL_BBOX_WIDTH;
	bottom = y + SHELL_SMALL_BBOX_HEIGHT;
}
