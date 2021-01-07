#include "BrickReward.h"
#include "GameObject.h"
#include "PlayScene.h"
#include "Coin.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "Point.h"

void CBrickReward::ShowReward() {
	if (this->type == BRICKREWARD_TYPE_COIN) {
		CCoin* coin = new CCoin();
		coin->SetPosition(this->x + 1.0f, this->y - 18.0f);
		coin->SetDefaultPosition(this->x + 1.0f, this->y - 18.0f);

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(8);
		coin->SetAnimationSet(ani_set);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(coin);
		coin->Jump();
		ShowPoint();
		CGame* game = CGame::GetInstance();
		game->PushCoin();
	}
	if (this->type == BRICKREWARD_TYPE_LEVEL) {
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
			CSuperMushroom* mushroom = new CSuperMushroom(SUPERMUSHROOM_TYPE_LEVEL);
			mushroom->SetPosition(this->x, this->y - 20.0f);
			mushroom->SetDefaultPosition(this->x, this->y - 20.0f);
			float mario_x, mario_y;
			mario->GetPosition(mario_x, mario_y);
			if (x + BRICKREWARD_BBOX_WIDTH * 0.5 > mario_x + MARIO_SMALL_BBOX_WIDTH * 0.5) {
				mushroom->SetSpeed(SUPERMUSHROOM_WALKING_SPEED, 0);
			}
			else {
				mushroom->SetSpeed(-SUPERMUSHROOM_WALKING_SPEED, 0);
			}

			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(12);
			mushroom->SetAnimationSet(ani_set);
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(mushroom);
		}
		else {
			CSuperLeaf* leaf = new CSuperLeaf();
			leaf->SetPosition(this->x, this->y - 18.0f);
			leaf->SetDefaultPosition(this->x, this->y - 18.0f);

			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(13);
			leaf->SetAnimationSet(ani_set);
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(leaf);
			leaf->Jump();
		}
		mario = NULL;
	}
}

void CBrickReward::ShowPoint() {
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(17);
	CPoint* point = new CPoint(1);
	point->SetPosition(x, y - 16.0f);
	point->SetAnimationSet(ani_set);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(point);
}

void CBrickReward::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (this->GetState() == BRICKREWARD_STATE_JUMP) {
		CGameObject::Update(dt);

		// Simple fall down
		vy += BRICKREWARD_GRAVITY * dt;
		y += dy;

		if (y > y_start) {
			y = y_start;
			vy = 0;
			this->SetState(BRICKREWARD_STATE_LOCK);
		}
	}
}

void CBrickReward::Render()
{
	int ani;
	ani = BRICKREWARD_ANI_IDLE;

	if (this->state != BRICKREWARD_STATE_IDLE)
		ani = BRICKREWARD_ANI_LOCK;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CBrickReward::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BRICKREWARD_STATE_IDLE:
		vy = 0;
		y = y_start;
		break;
	case BRICKREWARD_STATE_LOCK:
		vy = 0;
		y = y_start;
		break;
	case BRICKREWARD_STATE_JUMP:
		vy = -BRICKREWARD_JUMP_SPEED_Y;
		ShowReward();
		break;
	}
}

void CBrickReward::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BRICKREWARD_BBOX_WIDTH;
	bottom = y + BRICKREWARD_BBOX_HEIGHT;
}
