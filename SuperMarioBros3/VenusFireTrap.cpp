#include "VenusFireTrap.h"
#include "PlayScene.h"
#include "VenusFileTrapBullet.h"
#include "Utils.h"

bool CVenusFireTrap::CheckMarioInActiveZone() {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float l, t, r, b;
	mario->GetBoundingBox(l, t, r, b);

	if (r < x - VENUSFIRETRAP_SAFE_ZONE || l > x + VENUSFIRETRAP_BBOX_WIDTH + VENUSFIRETRAP_SAFE_ZONE) return true;
	return false;
}

void CVenusFireTrap::SetSide() {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->y >= this->y) {
		if (mario->x <= this->x) {
			side = VENUSFIRETRAP_SIDE_BOTTOM_LEFT;
		}
		else {
			side = VENUSFIRETRAP_SIDE_BOTTOM_RIGHT;
		}
	}
	else {
		if (mario->x <= this->x) {
			side = VENUSFIRETRAP_SIDE_TOP_LEFT;
		}
		else {
			side = VENUSFIRETRAP_SIDE_TOP_RIGHT;
		}
	}
}

void CVenusFireTrap::Shot() {

	CVenusFireTrapBullet* bullet = new CVenusFireTrapBullet();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	bullet->SetAnimationSet(animation_sets->Get(VENUSFIRETRAP_BULLET_ANI_SET_ID));
	bullet->SetPosition(x + 4.0f, y + 4.0f);
	
	switch (side)
	{
	case VENUSFIRETRAP_SIDE_BOTTOM_LEFT:
		bullet->SetSpeed(-VENUSFIRETRAP_BULLET_SPEED_X, VENUSFIRETRAP_BULLET_SPEED_Y);
		break;
	case VENUSFIRETRAP_SIDE_TOP_LEFT:
		bullet->SetSpeed(-VENUSFIRETRAP_BULLET_SPEED_X, -VENUSFIRETRAP_BULLET_SPEED_Y);
		break;
	case VENUSFIRETRAP_SIDE_TOP_RIGHT:
		bullet->SetSpeed(VENUSFIRETRAP_BULLET_SPEED_X, -VENUSFIRETRAP_BULLET_SPEED_Y);
		break;
	case VENUSFIRETRAP_SIDE_BOTTOM_RIGHT:
		bullet->SetSpeed(VENUSFIRETRAP_BULLET_SPEED_X, VENUSFIRETRAP_BULLET_SPEED_Y);
		break;
	}
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(bullet);
}

void CVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!this->isDisable) {
		// Calculate dx, dy 
		CGameObject::Update(dt);
		y += dy;

		SetSide();

		if (this->GetState() == VENUSFIRETRAP_STATE_IDLE && GetTickCount64() - last_rise >= VENUSFIRETRAP_COOLDOWNS && CheckMarioInActiveZone()) {
			this->SetState(VENUSFIRETRAP_STATE_UP);
		}

		if (this->GetState() == VENUSFIRETRAP_STATE_UP) {
			if (type == VENUSFIRETRAP_TYPE_GREEN && y <= this->y_start - VENUSFIRETRAP_GREEN_RISE_HEIGHT) {
				y = this->y_start - VENUSFIRETRAP_GREEN_RISE_HEIGHT;
				start_shot = GetTickCount64();
				this->SetState(VENUSFIRETRAP_STATE_SHOT);
				if (this->IsInCamera()) {
					Shot();
				}
			}
			if (type == VENUSFIRETRAP_TYPE_RED && y <= this->y_start - VENUSFIRETRAP_RED_RISE_HEIGHT) {
				y = this->y_start - VENUSFIRETRAP_RED_RISE_HEIGHT;
				start_shot = GetTickCount64();
				this->SetState(VENUSFIRETRAP_STATE_SHOT);
				if (this->IsInCamera()) {
					Shot();
				}
			}
		}

		if (this->GetState() == VENUSFIRETRAP_STATE_SHOT && GetTickCount64() - start_shot >= VENUSFIRETRAP_TIME_SHOT) {
			this->SetState(VENUSFIRETRAP_STATE_DOWN);
		}

		if (this->GetState() == VENUSFIRETRAP_STATE_DOWN && y >= this->y_start) {
			last_rise = GetTickCount64();
			this->SetState(VENUSFIRETRAP_STATE_IDLE);
		}

		if (this->isDisable == false && this->GetState() != VENUSFIRETRAP_STATE_IDLE && this->GetState() != VENUSFIRETRAP_STATE_DIE)
		{
			if (this->BeAttackByTail()) {
				this->SetState(VENUSFIRETRAP_STATE_DIE);
				ShowPoint();
				ShowTailAttackEffect();
			}
		}
	}
}

void CVenusFireTrap::Render()
{
	int ani;

	if (type == VENUSFIRETRAP_TYPE_GREEN)
	{
		if (this->GetState() == VENUSFIRETRAP_STATE_SHOT) {
			switch (side)
			{
			case VENUSFIRETRAP_SIDE_BOTTOM_LEFT:
				ani = VENUSFIRETRAP_ANI_GREEN_FIRE_BOTTOM_LEFT;
				break;
			case VENUSFIRETRAP_SIDE_TOP_LEFT:
				ani = VENUSFIRETRAP_ANI_GREEN_FIRE_TOP_LEFT;
				break;
			case VENUSFIRETRAP_SIDE_TOP_RIGHT:
				ani = VENUSFIRETRAP_ANI_GREEN_FIRE_TOP_RIGHT;
				break;
			case VENUSFIRETRAP_SIDE_BOTTOM_RIGHT:
				ani = VENUSFIRETRAP_ANI_GREEN_FIRE_BOTTOM_RIGHT;
				break;
			}
		}
		else {
			switch (side)
			{
			case VENUSFIRETRAP_SIDE_BOTTOM_LEFT:
				ani = VENUSFIRETRAP_ANI_GREEN_IDLE_BOTTOM_LEFT;
				break;
			case VENUSFIRETRAP_SIDE_TOP_LEFT:
				ani = VENUSFIRETRAP_ANI_GREEN_IDLE_TOP_LEFT;
				break;
			case VENUSFIRETRAP_SIDE_TOP_RIGHT:
				ani = VENUSFIRETRAP_ANI_GREEN_IDLE_TOP_RIGHT;
				break;
			case VENUSFIRETRAP_SIDE_BOTTOM_RIGHT:
				ani = VENUSFIRETRAP_ANI_GREEN_IDLE_BOTTOM_RIGHT;
				break;
			}
		}
	}
	else {
		if (this->GetState() == VENUSFIRETRAP_STATE_SHOT) {
			switch (side)
			{
			case VENUSFIRETRAP_SIDE_BOTTOM_LEFT:
				ani = VENUSFIRETRAP_ANI_RED_FIRE_BOTTOM_LEFT;
				break;
			case VENUSFIRETRAP_SIDE_TOP_LEFT:
				ani = VENUSFIRETRAP_ANI_RED_FIRE_TOP_LEFT;
				break;
			case VENUSFIRETRAP_SIDE_TOP_RIGHT:
				ani = VENUSFIRETRAP_ANI_RED_FIRE_TOP_RIGHT;
				break;
			case VENUSFIRETRAP_SIDE_BOTTOM_RIGHT:
				ani = VENUSFIRETRAP_ANI_RED_FIRE_BOTTOM_RIGHT;
				break;
			}
		}
		else {
			switch (side)
			{
			case VENUSFIRETRAP_SIDE_BOTTOM_LEFT:
				ani = VENUSFIRETRAP_ANI_RED_IDLE_BOTTOM_LEFT;
				break;
			case VENUSFIRETRAP_SIDE_TOP_LEFT:
				ani = VENUSFIRETRAP_ANI_RED_IDLE_TOP_LEFT;
				break;
			case VENUSFIRETRAP_SIDE_TOP_RIGHT:
				ani = VENUSFIRETRAP_ANI_RED_IDLE_TOP_RIGHT;
				break;
			case VENUSFIRETRAP_SIDE_BOTTOM_RIGHT:
				ani = VENUSFIRETRAP_ANI_RED_IDLE_BOTTOM_RIGHT;
				break;
			}
		}
	}

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CVenusFireTrap::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case VENUSFIRETRAP_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case VENUSFIRETRAP_STATE_UP:
		vx = 0;
		vy = -VENUSFIRETRAP_SPEED_Y;
		break;
	case VENUSFIRETRAP_STATE_SHOT:
		vx = 0;
		vy = 0;
		break;
	case VENUSFIRETRAP_STATE_DOWN:
		vx = 0;
		vy = VENUSFIRETRAP_SPEED_Y;
		break;
	case VENUSFIRETRAP_STATE_DIE:
		vx = 0;
		vy = 0;
		x = x_start;
		y = y_start;
		isDisable = true;
		break;
	}
}

void CVenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + VENUSFIRETRAP_BBOX_WIDTH;
	bottom = y + VENUSFIRETRAP_BBOX_HEIGHT;
}