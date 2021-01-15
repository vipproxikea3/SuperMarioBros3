#include "Station.h"
#include "MarioWorldMap.h"
#include "Utils.h"

void CStation::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
}

void CStation::Render()
{
	//animation_set->at(STATION_ANI_IDLE)->Render(x, y, 255);
}

void CStation::SetState(int state)
{
}

void CStation::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + STATION_BBOX_WIDTH;
	bottom = y + STATION_BBOX_HEIGHT;
}