#include "Curtain.h"

void CCurtain::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!this->isDisable) {
		// Calculate dx, dy 
		CGameObject::Update(dt);

		y += dy;

		if (y + CURTAIN_BBOX_HEIGHT < y_start) {
			y = y_start - CURTAIN_BBOX_HEIGHT;
			vy = 0;
		}
	}
}

void CCurtain::Render()
{
	int ani;
	ani = CURTAIN_ANI_IDLE;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CCurtain::SetState(int state)
{
}

void CCurtain::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CURTAIN_BBOX_WIDTH;
	bottom = y + CURTAIN_BBOX_HEIGHT;
}