#include "MarioFireBullet.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Shell.h"

void CMarioFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		float x0 = x;
		float y0 = y;

		x = x0 + dx;
		this->x = x0 + min_tx * this->dx + nx * 0.4f;

		if (!this->isDisable)
			for (UINT i = 0; i < coEventsResult.size(); i++) {
				LPCOLLISIONEVENT e = coEventsResult[i];

				// GOOMBA
				if (dynamic_cast<CGoomba*>(e->obj)) {
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
					}
				}

				// KOOPA
				if (dynamic_cast<CKoopa*>(e->obj))
				{
					CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
					if (koopa->GetState() != KOOPA_STATE_DIE)
					{
						koopa->SetState(KOOPA_STATE_DIE);
						koopa->showShell();
					}
				}

				// SHELL
				if (dynamic_cast<CShell*>(e->obj)) {
					CShell* shell = dynamic_cast<CShell*>(e->obj);
					if (shell->GetState() == SHELL_STATE_IDLE) {
						if (e->nx != 0) {
							if (e->nx < 0) {
								shell->SetState(SHELL_STATE_WALKING);
								shell->SetSpeed(SHELL_WALKING_SPEED, 0);
							}
							else {
								shell->SetState(SHELL_STATE_WALKING);
								shell->SetSpeed(-SHELL_WALKING_SPEED, 0);
							}
						}
					}
				}
			}

		this->isDisable = true;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMarioFireBullet::Render()
{
	int ani;
	ani = BULLET_ANI_WALKING_RIGHT;
	if (this->GetState() == BULLET_STATE_WALKING_LEFT)
		ani = BULLET_ANI_WALKING_LEFT;
	

	int alpha = 255;
	animations[ani]->Render(x, y, alpha);
}

void CMarioFireBullet::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BULLET_STATE_WALKING_RIGHT:
		vx = BULLET_WALKING_SPEED;
		break;
	case BULLET_STATE_WALKING_LEFT:
		vx = -BULLET_WALKING_SPEED;
		break;
	}
}

void CMarioFireBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y + BULLET_BBOX_HEIGHT;
}
