#include "SuperMushroom.h"
#include "Block.h"
#include "BrickReward.h"
#include "Game.h"
#include "BreakBlock.h"
#include "Point.h"
#include "PlayScene.h"

void CSuperMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!this->isDisable) {
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
						this->vx = 0;
						this->x = x0 + min_tx * this->dx + nx * 0.4f;
					}

					if (e->nx == 1 && block->isBlockRight()) {
						this->vx = 0;
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
				}

				// BREAKBLOCK
				if (dynamic_cast<CBreakBlock*>(e->obj)) {
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
				}

				// MARIO
				if (dynamic_cast<CMario*>(e->obj)) {
					CMario* mario = dynamic_cast<CMario*>(e->obj);
					this->isDisable = true;
					if (this->GetType() == SUPERMUSHROOM_TYPE_LEVEL)
					{
						mario->LvlUp();
					}
					if (this->GetType() == SUPERMUSHROOM_TYPE_LIFE) {
						CAnimationSets* animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(17);
						CPoint* point = new CPoint(0);
						point->SetPosition(x, y - 16.0f);
						point->SetAnimationSet(ani_set);
						((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(point);
						CGame* game = CGame::GetInstance();
						game->PushLifeStack();
					}
				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
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
	ani = SUPERMUSHROOM_LEVEL_ANI;
	if (type == SUPERMUSHROOM_TYPE_LIFE)
		ani = SUPERMUSHROOM_LIFE_ANI;
	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
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
	case SUPERMUSHROOM_STATE_WALKING:
		vy = 0;
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