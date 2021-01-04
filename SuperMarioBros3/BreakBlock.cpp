#include "BreakBlock.h"
#include "PlayScene.h"

void CBreakBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
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