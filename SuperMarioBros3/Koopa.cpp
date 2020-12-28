#include "Koopa.h"
#include "Block.h"

void CKoopa::setMyShell(CGameObject* shell) {
	myShell = shell;
	myShell->isDisable = true;
	myShell->SetPosition(this->x, this->y);
}

void CKoopa::showShell() {
	myShell->SetPosition(this->x, this->y);
	myShell->isDisable = false;
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += KOOPA_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (y > 500) {
		this->SetState(KOOPA_STATE_DIE);
	}

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

		if (!this->isDisable)
			for (UINT i = 0; i < coEventsResult.size(); i++) {
				LPCOLLISIONEVENT e = coEventsResult[i];

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
			}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (this->x <= this->limitL) {
		this->x = this->limitL;
		this->vx = -this->vx;
	}
	if (this->x >= this->limitR - KOOPA_BBOX_WIDTH) {
		this->x = this->limitR - KOOPA_BBOX_WIDTH;
		this->vx = -this->vx;
	}
}

void CKoopa::BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0)
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

void CKoopa::Render()
{
	int ani;
	if (vx > 0)
		ani = KOOPA_ANI_WALKING_RIGHT;
	else
		ani = KOOPA_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y, 255);
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_DIE:
		this->isDisable = true;
		break;
	case KOOPA_STATE_WALKING_LEFT:
		vx = -KOOPA_WALKING_SPEED;
		nx = -1;
		break;
	case KOOPA_STATE_WALKING_RIGHT:
		vx = KOOPA_WALKING_SPEED;
		nx = 1;
		break;
	}
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPA_BBOX_WIDTH;
	bottom = y + KOOPA_BBOX_HEIGHT;
}