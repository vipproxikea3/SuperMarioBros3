#include "PiranhaPlant.h"
#include "PlayScene.h"

bool CPiranhaPlant::CheckMarioInActiveZone() {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float l, t, r, b;
	mario->GetBoundingBox(l, t, r, b);

	if (r < x - PIRANHAPLANT_SAFE_ZONE || l > x + PIRANHAPLANT_BBOX_WIDTH + PIRANHAPLANT_SAFE_ZONE) return true;
	return false;
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!this->isDisable) {
		// Calculate dx, dy 
		CGameObject::Update(dt);
		y += dy;

		if (this->GetState() == PIRANHAPLANT_STATE_IDLE && GetTickCount64() - last_rise >= PIRANHAPLANT_COOLDOWNS && CheckMarioInActiveZone()) {
			this->SetState(PIRANHAPLANT_STATE_UP);
		}

		if (this->GetState() == PIRANHAPLANT_STATE_UP && y <= this->y_start - PIRANHAPLANT_RISE_HEIGHT) {
			y = this->y_start - PIRANHAPLANT_RISE_HEIGHT;
			start_bite = GetTickCount64();
			this->SetState(PIRANHAPLANT_STATE_BITE);
		}

		if (this->GetState() == PIRANHAPLANT_STATE_BITE && GetTickCount64() - start_bite >= PIRANHAPLANT_TIME_BITE) {
			this->SetState(PIRANHAPLANT_STATE_DOWN);
		}

		if (this->GetState() == PIRANHAPLANT_STATE_DOWN && y >= this->y_start) {
			last_rise = GetTickCount64();
			this->SetState(PIRANHAPLANT_STATE_IDLE);
		}

		if (this->isDisable == false && this->GetState() != PIRANHAPLANT_STATE_IDLE && this->GetState() != PIRANHAPLANT_STATE_DIE)
		{
			if (this->BeAttackByTail()) {
				this->SetState(PIRANHAPLANT_STATE_DIE);
				ShowPoint();
				ShowTailAttackEffect();
			}
		}
	}
}

void CPiranhaPlant::Render()
{
	int ani;
	ani = PIRANHAPLANT_ANI;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CPiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case PIRANHAPLANT_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case PIRANHAPLANT_STATE_UP:
		vx = 0;
		vy = -PIRANHAPLANT_SPEED_Y;
		break;
	case PIRANHAPLANT_STATE_BITE:
		vx = 0;
		vy = 0;
		break;
	case PIRANHAPLANT_STATE_DOWN:
		vx = 0;
		vy = PIRANHAPLANT_SPEED_Y;
		break;
	case PIRANHAPLANT_STATE_DIE:
		vx = 0;
		vy = 0;
		x = x_start;
		y = y_start;
		isDisable = true;
		break;
	}
}

void CPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHAPLANT_BBOX_WIDTH;
	bottom = y + PIRANHAPLANT_BBOX_HEIGHT;
}