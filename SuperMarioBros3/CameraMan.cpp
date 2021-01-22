#include "CameraMan.h"

void CCameraMan::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	// Calculate dx, dy 
	CGameObject::Update(dt);

	x += dx;
}

void CCameraMan::Render()
{
}

void CCameraMan::SetState(int state)
{
}

void CCameraMan::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}