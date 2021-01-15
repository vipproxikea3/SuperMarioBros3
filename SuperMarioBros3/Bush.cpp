#include "Bush.h"

void CBush::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
}

void CBush::Render()
{
	animation_set->at(BUSH_ANI_IDLE)->Render(x, y, 255);
}

void CBush::SetState(int state)
{
}

void CBush::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}