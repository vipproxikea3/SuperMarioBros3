#include "BreakBlock.h"
#include "PlayScene.h"

void CBreakBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (this->isDisable == false)
	{
		if (this->GetState() == BREAKBLOCK_STATE_IDLE) {
			if (this->BeAttackByTail()) {
				ShowReward();
			}
		}

		if (GetTickCount64() - start_damage > BREAK_BLOCK_DAMAGE_TIME && damaging) {
			damaging = false;
		}
	}
}

void CBreakBlock::ShowReward() {
	switch (type)
	{
	case BREAKBLOCK_TYPE_DEFAULT:
	{
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario->GetLevel() != MARIO_LEVEL_SMALL) {
			ShowPiece();
			this->isDisable = true;
		}
		break;
	}
	case BREAKBLOCK_TYPE_SWITCH:
		ShowSwitchBlock();
		this->SetState(BREAKBLOCK_STATE_LOCK);
		break;
	case BREAKBLOCK_TYPE_LIFE:
		ShowSuperMushroomLevel();
		this->SetState(BREAKBLOCK_STATE_LOCK);
		break;
	case BREAKBLOCK_TYPE_COIN:
		ShowCoin();
		if (countCoin == 0)
			this->SetState(BREAKBLOCK_STATE_LOCK);
		break;
	case BREAKBLOCK_TYPE_LEVEL:
		ShowLevelReward();
		this->SetState(BREAKBLOCK_STATE_LOCK);
		break;

	}
	StartDamage();
}

void CBreakBlock::ShowSwitchBlock() {
	CGameObject* obj = new CSwitchBlock();
	// General object setup
	obj->SetPosition(x, y - 16.0f);
	obj->SetDefaultPosition(x, y - 16.0f);

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(10);
	obj->SetAnimationSet(ani_set);

	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(obj);
}

void CBreakBlock::ShowPiece() {
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(16);

	CBreakBlockPiece* piece = new CBreakBlockPiece(BREAKBLOCKPIECE_TYPE_HIGH_LEFT);
	piece->SetPosition(this->x + 4.0f, this->y + 4.0f);
	piece->SetAnimationSet(ani_set);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(piece);

	piece = new CBreakBlockPiece(BREAKBLOCKPIECE_TYPE_LOW_LEFT);
	piece->SetPosition(this->x + 4.0f, this->y + 4.0f);
	piece->SetAnimationSet(ani_set);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(piece);

	piece = new CBreakBlockPiece(BREAKBLOCKPIECE_TYPE_HIGH_RIGHT);
	piece->SetPosition(this->x + 4.0f, this->y + 4.0f);
	piece->SetAnimationSet(ani_set);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(piece);

	piece = new CBreakBlockPiece(BREAKBLOCKPIECE_TYPE_LOW_RIGHT);
	piece->SetPosition(this->x + 4.0f, this->y + 4.0f);
	piece->SetAnimationSet(ani_set);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(piece);
}

void CBreakBlock::ShowSuperMushroomLevel() {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CSuperMushroom* mushroom = new CSuperMushroom(SUPERMUSHROOM_TYPE_LIFE);
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
	mario = NULL;
}

void CBreakBlock::ShowCoin() {
	CCoin* coin = new CCoin();
	coin->SetPosition(this->x, this->y - 18.0f);
	coin->SetDefaultPosition(this->x, this->y - 18.0f);

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(8);
	coin->SetAnimationSet(ani_set);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(coin);
	coin->Jump();
	ShowPoint();
	CBackup* backup = CBackup::GetInstance();
	backup->PushCoin();
	countCoin--;
}

void CBreakBlock::ShowLevelReward()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
		CSuperMushroom* mushroom = new CSuperMushroom(SUPERMUSHROOM_TYPE_LEVEL);
		mushroom->SetPosition(this->x, this->y - 17.0f);
		mushroom->SetDefaultPosition(this->x, this->y - 17.0f);
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

void CBreakBlock::StartDamage() {
	start_damage = GetTickCount64();
	damaging = true;
}

void CBreakBlock::Render()
{
	int ani;
	ani = BREAKBLOCK_ANI_IDLE;

	if (this->GetState() == BREAKBLOCK_STATE_LOCK)
		ani = BREAKBLOCK_ANI_LOCK;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CBreakBlock::SetState(int state)
{
	CGameObject::SetState(state);
}

void CBreakBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BREAKBLOCK_BBOX_WIDTH;
	bottom = y + BREAKBLOCK_BBOX_HEIGHT;
}