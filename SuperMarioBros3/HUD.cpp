#include "HUD.h"
#include "Mario.h"
#include "PlayScene.h"

void CHUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
}

void CHUD::Render()
{
	int ani;
	ani = HUD_ANI_BG;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);

	RenderRunSpeedStack();
}

void CHUD::RenderRunSpeedStack() {
	CMario * mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!mario) return;

	int runSpeedStack = mario->GetRunSpeedStack();

	for (int i = 0; i < runSpeedStack; i++) {
		if (i != 6) {
			int alpha = 255;
			animation_set->at(HUD_ANI_RUNSTACK_NORMAL)->Render(x + RUNSTACK_NORMAL_POS_X + i * RUNSTACK_NORMAL_WIDTH, y + RUNSTACK_NORMAL_POS_Y, alpha);
		}
	}

	if (runSpeedStack == 7)
		animation_set->at(HUD_ANI_RUNSTACK_MAX)->Render(x + RUNSTACK_MAX_POS_X, y + RUNSTACK_MAX_POS_Y, 255);

	mario = nullptr;
}

void CHUD::SetState(int state)
{
}

void CHUD::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}