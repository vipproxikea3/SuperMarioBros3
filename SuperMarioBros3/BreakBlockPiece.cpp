#include "BreakBlockPiece.h"
#include "Coin.h"

void CBreakBlockPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!this->isDisable) {
		if (GetTickCount64() - createdAt >= TIME_ALIVE)
			this->isDisable = true;
		CGameObject::Update(dt, coObjects);
		vy += BREAKBLOCKPIECE_GRAVITY * dt;
		x += dx;
		y += dy;
	}
}

void CBreakBlockPiece::Render()
{
	int ani;
	ani = BREAKBLOCKPIECE_ANI;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CBreakBlockPiece::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BREAKBLOCKPIECE_STATE_HIGH_JUMP_LEFT:
		vx = -BREAKBLOCKPIECE_WALKING_SPEED;
		vy = -BREAKBLOCKPIECE_HIGH_JUMP_SPEED;
		break;
	case BREAKBLOCKPIECE_STATE_HIGH_JUMP_RIGHT:
		vx = BREAKBLOCKPIECE_WALKING_SPEED;
		vy = -BREAKBLOCKPIECE_HIGH_JUMP_SPEED;
		break;
	case BREAKBLOCKPIECE_STATE_LOW_JUMP_LEFT:
		vx = -BREAKBLOCKPIECE_WALKING_SPEED;
		vy = -BREAKBLOCKPIECE_LOW_JUMP_SPEED;
		break;
	case BREAKBLOCKPIECE_STATE_LOW_JUMP_RIGHT:
		vx = BREAKBLOCKPIECE_WALKING_SPEED;
		vy = -BREAKBLOCKPIECE_LOW_JUMP_SPEED;
		break;
	}
}

void CBreakBlockPiece::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BREAKBLOCKPIECE_BBOX_WIDTH;
	bottom = y + BREAKBLOCKPIECE_BBOX_HEIGHT;
}