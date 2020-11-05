#include "Coin.h"

void CCoin::Render()
{
	int ani;
	ani = COIN_ANI_IDLE;

	int alpha = 255;
	animations[ani]->Render(x, y, alpha);
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + COIN_BBOX_WIDTH;
	bottom = y + COIN_BBOX_HEIGHT;
}