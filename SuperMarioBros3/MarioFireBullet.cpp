#include "MarioFireBullet.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Block.h"
#include "PiranhaPlant.h"
#include "VenusFireTrap.h"
#include "BrickReward.h"

void CMarioFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ReSet();
	if (!this->isDisable) {
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
						this->vy = -BULLET_JUMP_DEFLECT_SPEED;
						this->y = y0 + min_ty * this->dy + ny * 0.4f;
					}

					if (e->ny == 1 && block->isBlockBottom()) {
						this->vy = 0;
						this->y = y0 + min_ty * this->dy + ny * 0.4f;
						this->isDisable = true;
					}
				}

				// BREAKBLOCK
				if (dynamic_cast<CBreakBlock*>(e->obj)) {
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					if (e->nx != 0) {
						this->isDisable = true;
					}
					if (e->ny == -1) {
						this->vy = -BULLET_JUMP_DEFLECT_SPEED;
					}
				}

				// BRICKREWARD
				if (dynamic_cast<CBrickReward*>(e->obj)) {
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					if (e->nx != 0) {
						this->isDisable = true;
					}
					if (e->ny == -1) {
						this->vy = -BULLET_JUMP_DEFLECT_SPEED;
					}
				}

				// GOOMBA
				if (dynamic_cast<CGoomba*>(e->obj)) {
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					this->isDisable = true;
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					goomba->SetState(GOOMBA_STATE_DIE_X);
				}

				// PARAGOOMBA
				if (dynamic_cast<CParaGoomba*>(e->obj))
				{
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					this->isDisable = true;
					CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);
					if (paraGoomba->GetState() != PARAGOOMBA_STATE_DIE_X && paraGoomba->GetState() != PARAGOOMBA_STATE_DIE_Y)
					{
						paraGoomba->SetLevel(PARAGOOMBA_LEVEL_GOOMBA);
						paraGoomba->SetState(PARAGOOMBA_STATE_DIE_X);
					}
				}

				// KOOPA
				if (dynamic_cast<CKoopa*>(e->obj))
				{
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					this->isDisable = true;
					CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

					if (koopa->GetLevel() == KOOPA_LEVEL_KOOPA) {
						koopa->lvlDown();
					}
					else {
						if (koopa->GetState() == SHELL_STATE_IDLE) {
							if (e->nx != 0) {
								if (e->nx < 0) {
									koopa->SetState(SHELL_STATE_WALKING_RIGHT);
								}
								else {
									koopa->SetState(SHELL_STATE_WALKING_LEFT);
								}
							}
						}
					}

				}

				// PARAKOOPA
				if (dynamic_cast<CParaKoopa*>(e->obj))
				{
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					this->isDisable = true;
					CParaKoopa* paraKoopa = dynamic_cast<CParaKoopa*>(e->obj);
					if (paraKoopa->GetLevel() != PARAKOOPA_LEVEL_SHELL) {
						paraKoopa->lvlDown();
					}
					else {
						if (paraKoopa->GetState() == PARAKOOPA_SHELL_STATE_IDLE) {
							if (e->nx != 0) {
								if (e->nx < 0) {
									paraKoopa->SetState(PARAKOOPA_SHELL_STATE_WALKING_RIGHT);
								}
								else {
									paraKoopa->SetState(PARAKOOPA_SHELL_STATE_WALKING_LEFT);
								}
							}
						}
					}
				}

				// PIRANHAPLANT
				if (dynamic_cast<CPiranhaPlant*>(e->obj))
				{
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					this->isDisable = true;
					CPiranhaPlant* plant = dynamic_cast<CPiranhaPlant*>(e->obj);
					if (plant->isDisable == false && plant->GetState() != PIRANHAPLANT_STATE_IDLE && plant->GetState() != PIRANHAPLANT_STATE_DIE)
					{
						plant->SetState(PIRANHAPLANT_STATE_DIE);
						ShowPoint();
					}
				}

				// VENUSFIRETRAP
				if (dynamic_cast<CVenusFireTrap*>(e->obj))
				{
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					this->isDisable = true;
					CVenusFireTrap* trap = dynamic_cast<CVenusFireTrap*>(e->obj);
					if (trap->isDisable == false && trap->GetState() != VENUSFIRETRAP_STATE_IDLE && trap->GetState() != VENUSFIRETRAP_STATE_DIE)
					{
						trap->SetState(VENUSFIRETRAP_STATE_DIE);
						ShowPoint();
					}
				}

			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
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
	animation_set->at(ani)->Render(x, y, alpha);
}

void CMarioFireBullet::ReSet() {
	if (!this->IsInCamera()) {
		this->isDisable = true;
	}
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
