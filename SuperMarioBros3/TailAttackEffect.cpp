#include "TailAttackEffect.h"

void CTailAttackEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!isDisable)
	{
		if (GetTickCount64() - CreateAt > TAILATTACKEFFECT_TIME)
			this->isDisable = true;
	}
}

void CTailAttackEffect::Render()
{
	animation_set->at(TAILATTACKEFFECT_ANI_IDLE)->Render(x, y, 255);
}

void CTailAttackEffect::SetState(int state)
{
}

void CTailAttackEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + TAILATTACKEFFECT_BBOX_WIDTH;
	bottom = y + TAILATTACKEFFECT_BBOX_HEIGHT;
}