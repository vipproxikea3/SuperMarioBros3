#include "MarioFireBullet.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Shell.h"
#include "Block.h"

void CMarioFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += BULLET_GRAVITY * dt;

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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		float x0 = x;
		float y0 = y;

		x = x0 + dx;
		y = y0 + dy;

		/*this->x = x0 + min_tx * this->dx + nx * 0.4f;
		this->y = y0 + min_ty * this->dy + ny * 0.4f;*/

		if (!this->isDisable)
			for (UINT i = 0; i < coEventsResult.size(); i++) {
				LPCOLLISIONEVENT e = coEventsResult[i];

				// BLOCK
				if (dynamic_cast<CBlock*>(e->obj)) {
					CBlock* block = dynamic_cast<CBlock*>(e->obj);

					if (e->nx == -1 && block->isBlockLeft()) {
						this->vx = 0;
						this->x = x0 + min_tx * this->dx + nx * 0.4f;
						this->isDisable = true;
					}

					if (e->nx == 1 && block->isBlockRight()) {
						this->vx = 0;
						this->x = x0 + min_tx * this->dx + nx * 0.4f;
						this->isDisable = true;
					}

					if (e->ny == -1 && block->isBlockTop()) {
						this->vy = 0;
						this->y = y0 + min_ty * this->dy + ny * 0.4f;
						this->isDisable = true;
					}

					if (e->ny == 1 && block->isBlockBottom()) {
						this->vy = 0;
						this->y = y0 + min_ty * this->dy + ny * 0.4f;
						this->isDisable = true;
					}
				}

				// GOOMBA
				if (dynamic_cast<CGoomba*>(e->obj)) {
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					this->isDisable = true;
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
					}
				}

				// KOOPA
				if (dynamic_cast<CKoopa*>(e->obj))
				{
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					this->isDisable = true;
					CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
					if (koopa->GetState() != KOOPA_STATE_DIE)
					{
						koopa->SetState(KOOPA_STATE_DIE);
						koopa->showShell();
					}
				}

				// SHELL
				if (dynamic_cast<CShell*>(e->obj)) {
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					this->isDisable = true;
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
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMarioFireBullet::BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0)
{
	if (nx != 0)
	{
		this->vx = 0;
		this->x = x0 + min_tx * this->dx + nx * 0.4f;
	}
	if (ny != 0)
	{
		this->vy = 0;
		this->y = y0 + min_ty * this->dy + ny * 0.4f;
	}
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
