#include "BreakBlock.h"
#include "PlayScene.h"

void CBreakBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
}

void CBreakBlock::ShowReward() {
	switch (type)
	{
	case BREAKBLOCK_TYPE_SWITCH:
		ShowSwitchBlock();
		break;
	case BREAKBLOCK_TYPE_LIFE:
		ShowSuperMushroomLevel();
		break;
	}
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