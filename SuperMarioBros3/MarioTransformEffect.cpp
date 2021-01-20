#include "MarioTransformEffect.h"

void CMarioTransformEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!isDisable)
	{
		if (GetTickCount64() - CreateAt > MARIOTRANSFORMEFFECT_TIME)
			this->isDisable = true;
	}
}

void CMarioTransformEffect::Render()
{
	animation_set->at(MARIOTRANSFORMEFFECT_ANI_IDLE)->Render(x, y, 255);
}

void CMarioTransformEffect::SetState(int state)
{
}

void CMarioTransformEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MARIOTRANSFORMEFFECT_BBOX_WIDTH;
	bottom = y + MARIOTRANSFORMEFFECT_BBOX_HEIGHT;
}