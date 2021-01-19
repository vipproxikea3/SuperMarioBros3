#include "HUD.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Backup.h"

void CHUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
}

void CHUD::Render()
{
	int ani;
	ani = HUD_ANI_BG;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);

	RenderRunSpeedStack();
	RenderCoin();
	RenderRemainingTime();
	RenderLifeStack();
	RenderPoint();
	RenderLottery();
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

void CHUD::RenderCoin() {
	int ani_tens, ani_units;
	int coin, tens, units;

	CBackup* backup = CBackup::GetInstance();
	coin = backup->GetCoin();

	units = coin % 10;
	tens = coin / 10;
	if (tens > 9) tens = 9;

	switch (tens) {
	case 0:
		ani_tens = HUD_ANI_NUMBER_0;
		break;
	case 1:
		ani_tens = HUD_ANI_NUMBER_1;
		break;
	case 2:
		ani_tens = HUD_ANI_NUMBER_2;
		break;
	case 3:
		ani_tens = HUD_ANI_NUMBER_3;
		break;
	case 4:
		ani_tens = HUD_ANI_NUMBER_4;
		break;
	case 5:
		ani_tens = HUD_ANI_NUMBER_5;
		break;
	case 6:
		ani_tens = HUD_ANI_NUMBER_6;
		break;
	case 7:
		ani_tens = HUD_ANI_NUMBER_7;
		break;
	case 8:
		ani_tens = HUD_ANI_NUMBER_8;
		break;
	case 9:
		ani_tens = HUD_ANI_NUMBER_9;
		break;
	}
	animation_set->at(ani_tens)->Render(x + COIN_POS_X, y + COIN_POS_Y, 255);

	switch (units) {
	case 0:
		ani_units = HUD_ANI_NUMBER_0;
		break;
	case 1:
		ani_units = HUD_ANI_NUMBER_1;
		break;
	case 2:
		ani_units = HUD_ANI_NUMBER_2;
		break;
	case 3:
		ani_units = HUD_ANI_NUMBER_3;
		break;
	case 4:
		ani_units = HUD_ANI_NUMBER_4;
		break;
	case 5:
		ani_units = HUD_ANI_NUMBER_5;
		break;
	case 6:
		ani_units = HUD_ANI_NUMBER_6;
		break;
	case 7:
		ani_units = HUD_ANI_NUMBER_7;
		break;
	case 8:
		ani_units = HUD_ANI_NUMBER_8;
		break;
	case 9:
		ani_units = HUD_ANI_NUMBER_9;
		break;
	}
	animation_set->at(ani_units)->Render(x + COIN_POS_X + 8.0f, y + COIN_POS_Y, 255);
}

void CHUD::RenderRemainingTime() {
	CGame* game = CGame::GetInstance();
	DWORD start_game = game->GetStartGameTime();
	DWORD gamePlayTime = game->GetGamePlayTime();
	DWORD remainingTime = gamePlayTime - (GetTickCount64() - start_game);
	if (CGame::GetInstance()->GetCurrentSceneId() == 2) {
		remainingTime = 300000;
	}
	
	if (remainingTime % 1000 == 0) {
		remainingTime = remainingTime/1000;
	}
	else {
		remainingTime = remainingTime / 1000 + 1;
	}

	if (remainingTime < 0) remainingTime = 0;

	int ani_hundreds, ani_tens, ani_units;
	int hundreds, tens, units;

	hundreds = remainingTime / 100;
	tens = (remainingTime / 10) % 10;
	units = remainingTime % 10;

	switch (hundreds) {
	case 1:
		ani_hundreds = HUD_ANI_NUMBER_1;
		break;
	case 2:
		ani_hundreds = HUD_ANI_NUMBER_2;
		break;
	case 3:
		ani_hundreds = HUD_ANI_NUMBER_3;
		break;
	case 4:
		ani_hundreds = HUD_ANI_NUMBER_4;
		break;
	case 5:
		ani_hundreds = HUD_ANI_NUMBER_5;
		break;
	case 6:
		ani_hundreds = HUD_ANI_NUMBER_6;
		break;
	case 7:
		ani_hundreds = HUD_ANI_NUMBER_7;
		break;
	case 8:
		ani_hundreds = HUD_ANI_NUMBER_8;
		break;
	case 9:
		ani_hundreds = HUD_ANI_NUMBER_9;
		break;
	default:
		ani_hundreds = HUD_ANI_NUMBER_0;
		break;
	}
	animation_set->at(ani_hundreds)->Render(x + TIME_POS_X, y + TIME_POS_Y, 255);

	switch (tens) {
	case 0:
		ani_tens = HUD_ANI_NUMBER_0;
		break;
	case 1:
		ani_tens = HUD_ANI_NUMBER_1;
		break;
	case 2:
		ani_tens = HUD_ANI_NUMBER_2;
		break;
	case 3:
		ani_tens = HUD_ANI_NUMBER_3;
		break;
	case 4:
		ani_tens = HUD_ANI_NUMBER_4;
		break;
	case 5:
		ani_tens = HUD_ANI_NUMBER_5;
		break;
	case 6:
		ani_tens = HUD_ANI_NUMBER_6;
		break;
	case 7:
		ani_tens = HUD_ANI_NUMBER_7;
		break;
	case 8:
		ani_tens = HUD_ANI_NUMBER_8;
		break;
	case 9:
		ani_tens = HUD_ANI_NUMBER_9;
		break;
	}
	animation_set->at(ani_tens)->Render(x + TIME_POS_X + 8.0f, y + TIME_POS_Y, 255);

	switch (units) {
	case 0:
		ani_units = HUD_ANI_NUMBER_0;
		break;
	case 1:
		ani_units = HUD_ANI_NUMBER_1;
		break;
	case 2:
		ani_units = HUD_ANI_NUMBER_2;
		break;
	case 3:
		ani_units = HUD_ANI_NUMBER_3;
		break;
	case 4:
		ani_units = HUD_ANI_NUMBER_4;
		break;
	case 5:
		ani_units = HUD_ANI_NUMBER_5;
		break;
	case 6:
		ani_units = HUD_ANI_NUMBER_6;
		break;
	case 7:
		ani_units = HUD_ANI_NUMBER_7;
		break;
	case 8:
		ani_units = HUD_ANI_NUMBER_8;
		break;
	case 9:
		ani_units = HUD_ANI_NUMBER_9;
		break;
	}
	animation_set->at(ani_units)->Render(x + TIME_POS_X + 16.0f, y + TIME_POS_Y, 255);
}

void CHUD::RenderLifeStack() {
	int ani_tens, ani_units;
	int life, tens, units;

	life = CBackup::GetInstance()->GetLifeStack();

	units = life % 10;
	tens = life / 10;
	if (tens > 9) tens = 9;

	switch (tens) {
	case 0:
		ani_tens = HUD_ANI_NUMBER_0;
		break;
	case 1:
		ani_tens = HUD_ANI_NUMBER_1;
		break;
	case 2:
		ani_tens = HUD_ANI_NUMBER_2;
		break;
	case 3:
		ani_tens = HUD_ANI_NUMBER_3;
		break;
	case 4:
		ani_tens = HUD_ANI_NUMBER_4;
		break;
	case 5:
		ani_tens = HUD_ANI_NUMBER_5;
		break;
	case 6:
		ani_tens = HUD_ANI_NUMBER_6;
		break;
	case 7:
		ani_tens = HUD_ANI_NUMBER_7;
		break;
	case 8:
		ani_tens = HUD_ANI_NUMBER_8;
		break;
	case 9:
		ani_tens = HUD_ANI_NUMBER_9;
		break;
	}
	animation_set->at(ani_tens)->Render(x + LIFE_POS_X, y + LIFE_POS_Y, 255);

	switch (units) {
	case 0:
		ani_units = HUD_ANI_NUMBER_0;
		break;
	case 1:
		ani_units = HUD_ANI_NUMBER_1;
		break;
	case 2:
		ani_units = HUD_ANI_NUMBER_2;
		break;
	case 3:
		ani_units = HUD_ANI_NUMBER_3;
		break;
	case 4:
		ani_units = HUD_ANI_NUMBER_4;
		break;
	case 5:
		ani_units = HUD_ANI_NUMBER_5;
		break;
	case 6:
		ani_units = HUD_ANI_NUMBER_6;
		break;
	case 7:
		ani_units = HUD_ANI_NUMBER_7;
		break;
	case 8:
		ani_units = HUD_ANI_NUMBER_8;
		break;
	case 9:
		ani_units = HUD_ANI_NUMBER_9;
		break;
	}
	animation_set->at(ani_units)->Render(x + LIFE_POS_X + 8.0f, y + LIFE_POS_Y, 255);
}

void CHUD::RenderPoint() {
	CBackup* backup = CBackup::GetInstance();
	int point = backup->GetPoint();

	int pos_x = POINT_POS_X;
	for (int i = 1000000; i > 0; i = i / 10) {
		int number = (point / i) % 10;
		int ani;
		switch (number) {
		case 0:
			ani = HUD_ANI_NUMBER_0;
			break;
		case 1:
			ani = HUD_ANI_NUMBER_1;
			break;
		case 2:
			ani = HUD_ANI_NUMBER_2;
			break;
		case 3:
			ani = HUD_ANI_NUMBER_3;
			break;
		case 4:
			ani = HUD_ANI_NUMBER_4;
			break;
		case 5:
			ani = HUD_ANI_NUMBER_5;
			break;
		case 6:
			ani = HUD_ANI_NUMBER_6;
			break;
		case 7:
			ani = HUD_ANI_NUMBER_7;
			break;
		case 8:
			ani = HUD_ANI_NUMBER_8;
			break;
		case 9:
			ani = HUD_ANI_NUMBER_9;
			break;
		}
		animation_set->at(ani)->Render(x + pos_x, y + POINT_POS_Y, 255);
		pos_x += 8.0f;
	}
}

void CHUD::RenderLottery() {
	CBackup* backup = CBackup::GetInstance();
	int slot1 = backup->GetLotterySlot1();
	if (slot1 != 0) {
		switch (slot1)
		{
		case 100:
			animation_set->at(HUD_ANI_LOTTERY_MUSHROOM)->Render(x + LOTTERY_SLOT_1_POS_X, y + LOTTERY_SLOT_1_POS_Y, 255);
			break;
		case 200:
			animation_set->at(HUD_ANI_LOTTERY_FLOWER)->Render(x + LOTTERY_SLOT_1_POS_X, y + LOTTERY_SLOT_1_POS_Y, 255);
			break;
		case 300:
			animation_set->at(HUD_ANI_LOTTERY_STAR)->Render(x + LOTTERY_SLOT_1_POS_X, y + LOTTERY_SLOT_1_POS_Y, 255);
			break;
		}
	}
	int slot2 = backup->GetLotterySlot2();
	if (slot2 != 0) {
		switch (slot2)
		{
		case 100:
			animation_set->at(HUD_ANI_LOTTERY_MUSHROOM)->Render(x + LOTTERY_SLOT_2_POS_X, y + LOTTERY_SLOT_2_POS_Y, 255);
			break;
		case 200:
			animation_set->at(HUD_ANI_LOTTERY_FLOWER)->Render(x + LOTTERY_SLOT_2_POS_X, y + LOTTERY_SLOT_2_POS_Y, 255);
			break;
		case 300:
			animation_set->at(HUD_ANI_LOTTERY_STAR)->Render(x + LOTTERY_SLOT_2_POS_X, y + LOTTERY_SLOT_2_POS_Y, 255);
			break;
		}
	}
	int slot3 = backup->GetLotterySlot3();
	if (slot3 != 0) {
		switch (slot3)
		{
		case 100:
			animation_set->at(HUD_ANI_LOTTERY_MUSHROOM)->Render(x + LOTTERY_SLOT_3_POS_X, y + LOTTERY_SLOT_3_POS_Y, 255);
			break;
		case 200:
			animation_set->at(HUD_ANI_LOTTERY_FLOWER)->Render(x + LOTTERY_SLOT_3_POS_X, y + LOTTERY_SLOT_3_POS_Y, 255);
			break;
		case 300:
			animation_set->at(HUD_ANI_LOTTERY_STAR)->Render(x + LOTTERY_SLOT_3_POS_X, y + LOTTERY_SLOT_3_POS_Y, 255);
			break;
		}
	}
}

void CHUD::SetState(int state)
{
}

void CHUD::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}