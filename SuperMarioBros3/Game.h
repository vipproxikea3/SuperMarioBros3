#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#define DIRECTINPUT_VERSION 0x0800
#define GAMEPLAY_TIME	300000
#include <dinput.h>

#include "Scene.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024

class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height;

	int coin = 0;
	DWORD start_game;
	int lifeStack = 4;
	int point = 0;
	int lotterySlot1 = 0;
	int lotterySlot2 = 0;
	int lotterySlot3 = 0;

	unordered_map<int, LPSCENE> scenes;
	int current_scene;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

public:
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	int GetCurrentSceneId() { return current_scene; }
	void SwitchScene(int scene_id);

	float GetScreenWidth() { return screen_width; }
	float GetScreenHeight() { return screen_height; }

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
	DWORD GetStartGameTime() { return start_game; }
	DWORD GetGamePlayTime() { return GAMEPLAY_TIME; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y)
	{
		cam_x = round(x);
		cam_y = round(y);
	}
	void GetCamPos(float& x, float& y) { x = this->cam_x; y = this->cam_y; }
	bool IsInCamera(float l, float t, float r, float b) {
		float camX, camY;
		float screenWidth, screenHeight;
		GetCamPos(camX, camY);
		screenWidth = GetScreenWidth();
		screenHeight = GetScreenHeight();

		if (r < camX) return false;
		if (l > camX + screenWidth) return false;
		return true;
	}

	static CGame* GetInstance();

	~CGame();
};


