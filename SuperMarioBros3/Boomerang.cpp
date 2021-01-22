#include "Boomerang.h"
#include "PlayScene.h"

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!this->isDisable) {
		// Calculate dx, dy 
		CGameObject::Update(dt);

		switch (this->GetState())
		{
		case BOOMERANG_STATE_WALKING_LEFT_PHARSE_1:
			if (y < y_start - BOOMERANG_FLY_ZONE)
			{
				y = y_start - BOOMERANG_FLY_ZONE;
				SetState(BOOMERANG_STATE_WALKING_LEFT_PHARSE_2);
			}
			break;
		case BOOMERANG_STATE_WALKING_LEFT_PHARSE_2:
			if (y > y_start)
			{
				y = y_start;
				SetState(BOOMERANG_STATE_WALKING_LEFT_PHARSE_3);
			}
			break;
		case BOOMERANG_STATE_WALKING_RIGHT_PHARSE_1:
			if (y < y_start - BOOMERANG_FLY_ZONE)
			{
				y = y_start - BOOMERANG_FLY_ZONE;
				SetState(BOOMERANG_STATE_WALKING_RIGHT_PHARSE_2);
			}
			break;
		case BOOMERANG_STATE_WALKING_RIGHT_PHARSE_2:
			if (y > y_start)
			{
				y = y_start;
				SetState(BOOMERANG_STATE_WALKING_RIGHT_PHARSE_3);
			}
			break;
		}

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

				//MARIO
				if (dynamic_cast<CMario*>(e->obj)) {
					this->isDisable = true;
					CMario* mario = dynamic_cast<CMario*>(e->obj);
					mario->lvlDown();
				}

				// BROTHER
				if (dynamic_cast<CBoomerangBrother*>(e->obj)) {
					this->isDisable = true;
					CBoomerangBrother* brother = dynamic_cast<CBoomerangBrother*>(e->obj);
					if (brother->GetState() == BOOMERANGBROTHER_STATE_ATTACK_LEFT)
					{
						brother->SetState(BOOMERANGBROTHER_STATE_READY_LEFT);
					}
					if (brother->GetState() == BOOMERANGBROTHER_STATE_ATTACK_RIGHT)
					{
						brother->SetState(BOOMERANGBROTHER_STATE_READY_RIGHT);
					}


				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CBoomerang::Render()
{
	int ani;
	
	if (state == BOOMERANG_STATE_WALKING_LEFT_PHARSE_1 || state == BOOMERANG_STATE_WALKING_LEFT_PHARSE_2 || state == BOOMERANG_STATE_WALKING_LEFT_PHARSE_3)
	{
		ani = BOOMERANG_ANI_WALKING_LEFT;
	}
	else {
		ani = BOOMERANG_ANI_WALKING_RIGHT;
	}

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CBoomerang::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BOOMERANG_STATE_WALKING_LEFT_PHARSE_1:
		vx = -BOOMERANG_WALKING_SPEED;
		vy = -BOOMERANG_FLY_SPEED_PHARSE_1;
		break;
	case BOOMERANG_STATE_WALKING_LEFT_PHARSE_2:
		vx = -BOOMERANG_WALKING_SPEED;
		vy = BOOMERANG_FLY_SPEED_PHARSE_2;
		break;
	case BOOMERANG_STATE_WALKING_LEFT_PHARSE_3:
		vx = BOOMERANG_WALKING_SPEED;
		vy = BOOMERANG_FLY_SPEED_PHARSE_3;
		break;
	case BOOMERANG_STATE_WALKING_RIGHT_PHARSE_1:
		vx = BOOMERANG_WALKING_SPEED;
		vy = -BOOMERANG_FLY_SPEED_PHARSE_1;
		break;
	case BOOMERANG_STATE_WALKING_RIGHT_PHARSE_2:
		vx = BOOMERANG_WALKING_SPEED;
		vy = BOOMERANG_FLY_SPEED_PHARSE_2;
		break;
	case BOOMERANG_STATE_WALKING_RIGHT_PHARSE_3:
		vx = -BOOMERANG_WALKING_SPEED;
		vy = BOOMERANG_FLY_SPEED_PHARSE_3;
		break;
	}
}

void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BBOX_WIDTH;
	bottom = y + BOOMERANG_BBOX_HEIGHT;
}