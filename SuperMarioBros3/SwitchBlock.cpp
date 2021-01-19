#include "SwitchBlock.h"
#include "PlayScene.h"

void CSwitchBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

}

void CSwitchBlock::Switch() {
	this->SetState(SWITCHBLOCK_STATE_ACTIVED);
	vector<LPGAMEOBJECT> objects = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetListObj();
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CBreakBlock*>(objects[i])) {
			CBreakBlock* breakBlock = dynamic_cast<CBreakBlock*>(objects[i]);
			if (breakBlock->isDisable == false && breakBlock->GetType() == BREAKBLOCK_TYPE_DEFAULT && breakBlock->GetState() == BREAKBLOCK_STATE_IDLE) {
				CCoin* coin = new CCoin();
				coin->SetPosition(breakBlock->x, breakBlock->y);
				coin->SetDefaultPosition(breakBlock->x, breakBlock->y);
				coin->SetStartComvert();

				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set = animation_sets->Get(8);
				coin->SetAnimationSet(ani_set);

				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(coin);
				breakBlock->isDisable = true;
			}
		}
	}
}

void CSwitchBlock::Render()
{
	int ani;
	ani = SWITCHBLOCK_ANI_IDLE;

	if (this->GetState() == SWITCHBLOCK_STATE_ACTIVED)
		ani = SWITCHBLOCK_ANI_ACTIIVED;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CSwitchBlock::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == SWITCHBLOCK_STATE_ACTIVED) {
		y += SWITCHBLOCK_BBOX_HEIGHT - SWITCHBLOCK_ACTIVED_BBOX_HEIGHT;
	}
}

void CSwitchBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SWITCHBLOCK_BBOX_WIDTH;
	bottom = y + SWITCHBLOCK_BBOX_HEIGHT;
	if (this->GetState() == SWITCHBLOCK_STATE_ACTIVED) {
		top = y + SWITCHBLOCK_BBOX_HEIGHT;
		bottom = y + SWITCHBLOCK_BBOX_HEIGHT;
	}
}