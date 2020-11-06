#include "Koopa.h"

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

	//Revival();

	x += dx;
	if (x <= limitL) {
		x = limitL;
		this->SetState(KOOPA_STATE_WALKING_RIGHT);
	}
	if (x >= limitR - KOOPA_BBOX_WIDTH) {
		x = limitR - KOOPA_BBOX_WIDTH;
		this->SetState(KOOPA_STATE_WALKING_LEFT);
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

	animations[ani]->Render(x, y);
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