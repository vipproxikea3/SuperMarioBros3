#include "Coin.h"
#include "PlayScene.h"

void CCoin::Jump() {
	this->SetState(COIN_STATE_JUMP);
}

void CCoin::SetStartComvert() {
	start_convert = GetTickCount64();
	CanConvert = true;
}

void CCoin::ConvertToBreakBlock() {
	CBreakBlock* brick = new CBreakBlock(0);
	brick->SetPosition(this->x, this->y);
	brick->SetDefaultPosition(this->x, this->y);

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(9);
	brick->SetAnimationSet(ani_set);

	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(brick);
	this->isDisable = true;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!this->isDisable) {
		// Calculate dx, dy 
		CGameObject::Update(dt);

		if (this->state == COIN_STATE_JUMP) {
			// Simple fall down
			vy += COIN_GRAVITY * dt;

			y += dy;

			if (y > y_start) isDisable = true;
		}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
			y += dy;
			x += dx;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny = 0;
			float rdx = 0;
			float rdy = 0;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
			float x0 = x;
			float y0 = y;

			x = x0 + dx;
			y = y0 + dy;

			for (UINT i = 0; i < coEventsResult.size(); i++) {
				LPCOLLISIONEVENT e = coEventsResult[i];

				//MARIO
				if (dynamic_cast<CMario*>(e->obj)) {
					this->isDisable = true;
					CBackup* backup = CBackup::GetInstance();
					backup->PushCoin();
				}
			}
		}

		if (this->isDisable == false && GetTickCount64() - start_convert >= COIN_TIME_CONVERT && CanConvert == true)
			ConvertToBreakBlock();

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CCoin::Render()
{
	int ani;
	ani = COIN_ANI_IDLE;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case COIN_STATE_IDLE:
			vy = 0;
		break;
	case COIN_STATE_JUMP:
		vy = -COIN_JUMP_SPEED_Y;
		break;
	}
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + COIN_BBOX_WIDTH;
	bottom = y + COIN_BBOX_HEIGHT;
}