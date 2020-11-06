#include "SuperMushroom.h"
#include "Block.h"
#include "BrickReward.h"
#include "Game.h"

void CSuperMushroom::showReward() {
	this->SetState(SUPERMUSHROOM_STATE_WALKING_LEFT);
}

void CSuperMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
 	vy += SUPERMUSHROOM_GRAVITY * dt;

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
		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			// BLOCK
			if (dynamic_cast<CBlock*>(e->obj)) {
				if (e->ny == -1)
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
			}

			// BRICKREWARD
			if (dynamic_cast<CBrickReward*>(e->obj)) {
				if (e->ny == -1)
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
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

void CSuperMushroom::BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0)
{
	if (ny != 0)
	{
		this->vy = 0;
		this->y = y0 + min_ty * this->dy + ny * 0.1f;
	}
}

void CSuperMushroom::Render()
{
	int ani;
	ani = SUPERMUSHROOM_ANI_SMALL_IDLE;
	int alpha = 255;
	animations[ani]->Render(x, y, alpha);
}

void CSuperMushroom::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SUPERMUSHROOM_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case SUPERMUSHROOM_STATE_WALKING_RIGHT:
		vy = -0.2f;
		vx = SUPERMUSHROOM_WALKING_SPEED;
		break;
	case SUPERMUSHROOM_STATE_WALKING_LEFT:
		vy = -0.2f;
		vx = -SUPERMUSHROOM_WALKING_SPEED;
		break;
	}
}

void CSuperMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SUPERMUSHROOM_BBOX_WIDTH;
	bottom = y + SUPERMUSHROOM_BBOX_HEIGHT;
}