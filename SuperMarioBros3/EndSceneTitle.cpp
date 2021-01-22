#include "EndSceneTitle.h"
#include "Mario.h"
#include "PlayScene.h"

void CEndSceneTitle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!this->isDisable) {
		if (GetTickCount64() - start_show > ENDSCENETITLE_TIME_SHOW) {
			showing = true;
		}
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario && !mario->IsReadyOutScene()) {
			mario->ReadyOutScene();
		}
	}
}

void CEndSceneTitle::Render()
{
	if (!showing)
		return;

	int ani;

	switch (type)
	{
	case ENDSCENETITLE_TYPE_MUSHROOM:
		ani = ENDSCENETITLE_ANI_MUSHROOM;
		break;
	case ENDSCENETITLE_TYPE_FLOWER:
		ani = ENDSCENETITLE_ANI_FLOWER;
		break;
	case ENDSCENETITLE_TYPE_STAR:
		ani = ENDSCENETITLE_ANI_STAR;
		break;
	}
	int alpha = 255;
	animation_set->at(ani)->Render(x + ENDSCENETITLE_CARD_POS_X, y + ENDSCENETITLE_CARD_POS_Y, alpha);
	animation_set->at(ENDSCENETITLE_ANI_BG)->Render(x, y, alpha);
}

void CEndSceneTitle::SetState(int state)
{
}

void CEndSceneTitle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ENDSCENETITLE_BBOX_WIDTH;
	bottom = y + ENDSCENETITLE_BBOX_HEIGHT;
}