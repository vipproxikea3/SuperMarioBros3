#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Utils.h"

class CBackup
{
	float mWlastPosX = 57;
	float mWlastPosY = 58;
	float mWcurPosX = 57;
	float mWcurPosY = 58;
	bool mWlastCanLeft = false;
	bool mWlastCanTop = false;
	bool mWlastCanRight = true;
	bool mWlastCanBot = false;
	bool mWcurCanLeft = false;
	bool mWcurCanTop = false;
	bool mWcurCanRight = true;
	bool mWcurCanBot = false;

	int marioLevel = 1;
	int coin = 0;
	int lifeStack = 4;
	int point = 0;
	int lotterySlot1 = 0;
	int lotterySlot2 = 0;
	int lotterySlot3 = 0;

	static CBackup* __instance;
public:

	float GetmWlastPosX() { return mWlastPosX; }
	float GetmWlastPosY() { return mWlastPosY;}
	float GetmWcurPosX() { return mWcurPosX;}
	float GetmWcurPosY() { return mWcurPosY;}
	float GetmWlastCanLeft() { return mWlastCanLeft;}
	float GetmWlastCanTop() { return mWlastCanTop;}
	float GetmWlastCanRight() { return mWlastCanRight;}
	float GetmWlastCanBot() { return mWlastCanBot;}
	float GetmWcurCanLeft() { return mWcurCanLeft;}
	float GetmWcurCanTop() { return mWcurCanTop;}
	float GetmWcurCanRight() { return mWcurCanRight;}
	float GetmWcurCanBot() { return mWcurCanBot;}

	void SetLastPos(float x, float y)
	{
		mWlastPosX = x;
		mWlastPosY = y;
	}

	void SetCurPos(float x, float y)
	{
		mWcurPosX = x;
		mWcurPosY = y;
	}

	void SetLastControl(bool l, bool t, bool r, bool b) {
		mWlastCanLeft = l;
		mWlastCanTop = t;
		mWlastCanRight = r;
		mWlastCanBot = b;
	}

	void SetCurControl(bool l, bool t, bool r, bool b) {
		mWcurCanLeft = l;
		mWcurCanTop = t;
		mWcurCanRight = r;
		mWcurCanBot = b;
	}

	int GetMarioLevel() { return marioLevel; }
	void SetMarioLevel(int level) { marioLevel = level; }

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
		if (lottery_type == 0)
		{
			lotterySlot1 = 0;
			lotterySlot1 = 0;
			lotterySlot1 = 0;
		}
		else
		{
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
	}

	static CBackup* GetInstance();

	~CBackup();
};

