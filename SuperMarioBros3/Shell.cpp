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

		if (!this->isDisable)
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				// BLOCK
				if (dynamic_cast<CBlock*>(e->obj)) {
					CBlock* block = dynamic_cast<CBlock*>(e->obj);
					switch (block->GetTypeBlock()) {
					case 0:
						BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
						if (e->nx != 0)
							switch (this->nx)
							{
							case -1:
								this->SetState(SHELL_STATE_WALKING_RIGHT);
								break;
							case 1:
								this->SetState(SHELL_STATE_WALKING_LEFT);
								break;
							}
					case 1:
						if (e->ny == -1)
							BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
						break;
					}
				}

				// BRICKREWARD
				if (dynamic_cast<CBrickReward*>(e->obj)) {
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					if (e->nx != 0)
						switch (this->nx)
						{
						case -1:
							this->SetState(SHELL_STATE_WALKING_RIGHT);
							break;
						case 1:
							this->SetState(SHELL_STATE_WALKING_LEFT);
							break;
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
					goomba->SetState(GOOMBA_STATE_DIE);
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
	ani = SHELL_ANI_IDLE;

	if (this->GetState() != SHELL_ANI_IDLE)
		ani = SHELL_ANI_SMALL_WALKING;

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
	case SHELL_STATE_WALKING_RIGHT:
		vy = 0;
		nx = 1;
		vx = SHELL_WALKING_SPEED;
		break;
	case SHELL_STATE_WALKING_LEFT:
		vy = 0;
		nx = -1;
		vx = -SHELL_WALKING_SPEED;
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
