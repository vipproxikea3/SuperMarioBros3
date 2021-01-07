#include "Point.h"
#include "PlayScene.h"

void CPoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!this->isDisable) {
		if (GetTickCount64() - createdAt >= TIME_ALIVE)
			this->isDisable = true;
	}
}

void CPoint::Render()
{
	int ani;
	
	switch (this->type)
	{
	case POINT_TYPE_LVLUP:
		ani = POINT_ANI_LVLUP;
		break;
	case POINT_TYPE_100:
		ani = POINT_ANI_100;
		break;
	}

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CPoint::SetState(int state)
{
}

void CPoint::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + POINT_BBOX_WIDTH;
	bottom = y + POINT_BBOX_HEIGHT;
}