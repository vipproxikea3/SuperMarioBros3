#include "VenusFileTrapBullet.h"
#include "Mario.h"

void CVenusFireTrapBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!this->isDisable) {
		if (!this->IsInCamera()) {
			this->isDisable = true;
		}

		CGameObject::Update(dt, coObjects);

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
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

				// MARIO
				if (dynamic_cast<CMario*>(e->obj))
				{
					this->isDisable = true;
					CMario* mario = dynamic_cast<CMario*>(e->obj);
					if (!mario->IsUntouchable()) {
						mario->lvlDown();
					}
				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CVenusFireTrapBullet::Render()
{
	int ani;

	ani = VENUSFIRETRAPBULLET_ANI;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CVenusFireTrapBullet::SetState(int state)
{
}

void CVenusFireTrapBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + VENUSFIRETRAPBULLET_BBOX_WIDTH;
	bottom = y + VENUSFIRETRAPBULLET_BBOX_HEIGHT;
}