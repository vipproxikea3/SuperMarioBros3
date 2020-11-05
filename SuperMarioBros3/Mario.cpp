#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "Block.h"
#include "Goomba.h"
#include "Coin.h"
#include "BrickReward.h"

//#include "Goomba.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->ny == -1)
				canJump = 1;
			

			// BLOCK
			if (dynamic_cast<CBlock*>(e->obj)) {
				CBlock* block = dynamic_cast<CBlock*>(e->obj);
				switch (block->GetTypeBlock()) {
				case 0:
					BasicCollision(min_tx, min_ty, nx, ny, x0, y0);
					break;
				case 1:
					if (e->ny == -1)
						BasicCollision(min_tx, min_ty, nx, ny, x0, y0);
					break;
				}
			}
			
			// GOOMBA
			if (dynamic_cast<CGoomba*>(e->obj))
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (e->ny != 0) {
					vy = 0;
					y = y0 + min_ty * dy + e->ny * 0.4f;
					if (e->ny < 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
					else {
						if (untouchable == 0)
						{
							if (goomba->GetState() != GOOMBA_STATE_DIE)
							{
								if (level > MARIO_LEVEL_SMALL)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
					}
				}

				if (e->nx != 0) {
					vx = 0;
					x = x0 + min_tx * dx + e->nx * 0.4f;
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}

			if (dynamic_cast<CCoin*>(e->obj)) {
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				if (coin->GetState() == COIN_STATE_IDLE) {
					coin->isDisable = true;
				}
			}

			if (dynamic_cast<CBrickReward*>(e->obj)) {
				CBrickReward* brickReward = dynamic_cast<CBrickReward*>(e->obj);
				BasicCollision(min_tx, min_ty, nx, ny, x0, y0);
				if (e->ny == 1 && brickReward->GetState() == BRICKREWARD_STATE_IDLE) {
					brickReward->SetState(BRICKREWARD_STATE_JUMP);
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0)
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
		/*if (ny == -1)
			this->ny = 0;*/
	}
}

void CMario::Render()
{
	int ani;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else
		if (level == MARIO_LEVEL_BIG)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else ani = MARIO_ANI_BIG_WALKING_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (vy < 0) {
				if (nx > 0)
					ani = MARIO_ANI_SMALL_JUMP_RIGHT;
				else
					ani = MARIO_ANI_SMALL_JUMP_LEFT;
			}
			else {
				if (vx == 0)
				{
					if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
					else ani = MARIO_ANI_SMALL_IDLE_LEFT;
				}
				else if (vx > 0)
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				else ani = MARIO_ANI_SMALL_WALKING_LEFT;
			}
		}

	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(x, y, alpha);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		vy = -MARIO_JUMP_SPEED_Y;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

