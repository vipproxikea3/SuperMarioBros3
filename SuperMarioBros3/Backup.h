#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class CBackup
{
	int coin = 0;
	int lifeStack = 4;
	int point = 0;
	int lotterySlot1 = 0;
	int lotterySlot2 = 0;
	int lotterySlot3 = 0;

	static CBackup* __instance;
public:
	int GetCoin() { return coin; }
	void SetCoin(int coin) { this->coin = coin; }
	void PushCoin() { coin = coin + 1; }

	int GetLifeStack() { return lifeStack; }
	void SetLifeStack(int lifeStack) { this->lifeStack = lifeStack; }
	void PushLifeStack() { lifeStack = lifeStack + 1; }

	int GetPoint() { return point; }
	void SetPoint(int point) { this->point = point; }
	void PushPoint(int pushPoint) { point += pushPoint; }
	void PopLifeStack() { lifeStack = lifeStack - 1; }

	int GetLotterySlot1() { return lotterySlot1; }
	int GetLotterySlot2() { return lotterySlot2; }
	int GetLotterySlot3() { return lotterySlot3; }
	void SetLottery(int lottery_type) {
		if (lotterySlot1 == 0) {
			switch (lottery_type)
			{
			case 100:
				lotterySlot1 = 100;
				break;
			case 200:
				lotterySlot1 = 200;
				break;
			case 300:
				lotterySlot1 = 300;
				break;
			}
			return;
		}
		else if (lotterySlot2 == 0) {
			switch (lottery_type)
			{
			case 100:
				lotterySlot2 = 100;
				break;
			case 200:
				lotterySlot2 = 200;
				break;
			case 300:
				lotterySlot2 = 300;
				break;
			}
			return;
		}
		else if (lotterySlot3 == 0) {
			switch (lottery_type)
			{
			case 100:
				lotterySlot3 = 100;
				break;
			case 200:
				lotterySlot3 = 200;
				break;
			case 300:
				lotterySlot3 = 300;
				break;
			}
			return;
		}
	}

	static CBackup* GetInstance();

	~CBackup();
};

