#include "Gate.h"

void CGate::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
}

void CGate::Render()
{
	int ani;
	ani = GATE_ANI;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CGate::SetState(int state)
{
}

void CGate::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GATE_BBOX_WIDTH;
	bottom = y + GATE_BBOX_HEIGHT;
}