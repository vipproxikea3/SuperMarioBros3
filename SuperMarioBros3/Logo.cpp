#include "Logo.h"

void CLogo::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
}

void CLogo::Render()
{
	animation_set->at(LOGO_ANI_IDLE)->Render(x, y, 255);
}

void CLogo::SetState(int state)
{
}

void CLogo::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + LOGO_BBOX_WIDTH;
	bottom = y + LOGO_BBOX_HEIGHT;
}