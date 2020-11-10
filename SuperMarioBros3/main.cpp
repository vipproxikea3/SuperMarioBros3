#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Map.h"
#include "Mario.h"
#include "Block.h"
#include "Goomba.h"
//#include "Coin.h"
#include "BrickReward.h"
//#include "SuperMushroom.h"
#include "Shell.h"
#include "Koopa.h"

#define WINDOW_CLASS_NAME L"SuperMarioBros3"
#define MAIN_WINDOW_TITLE L"SuperMarioBros3"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240

#define MAX_FRAME_RATE	120

#define ID_TEX_MAP		0
#define ID_TEX_MARIO	10
#define ID_TEX_ENEMY	20
#define ID_TEX_ENEMY_2	30
#define ID_TEX_MISC		40

CGame* game;
Map* map;

CMario* mario;
CBlock* block;
CGoomba* goomba;
//CCoin* coin;
CBrickReward* brickReward;
//CSuperMushroom* superMushroom;
CShell* shell;
CKoopa* koopa;

//vector<LPGAMEOBJECT> superMushrooms;
//vector<LPGAMEOBJECT> coins;
vector<LPGAMEOBJECT> brickRewards;
vector<LPGAMEOBJECT> blocks;
vector<LPGAMEOBJECT> objects;

class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander* keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	if (mario->state != MARIO_STATE_DIE) {
		switch (KeyCode)
		{
		case DIK_SPACE:
			if (mario->canJump) {
				mario->SetState(MARIO_STATE_JUMP);
				mario->canJump = false;
			}
			break;
		case DIK_A:
			mario->LvlUp();
			break;
		}
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE* states)
{
	if (mario->state != MARIO_STATE_DIE) {
		// disable control key when Mario die 
		if (mario->GetState() == MARIO_STATE_DIE) return;
		if (game->IsKeyDown(DIK_RIGHT))
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		else if (game->IsKeyDown(DIK_LEFT))
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		else
			mario->SetState(MARIO_STATE_IDLE);
	}
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();
	textures->Add(ID_TEX_MAP, L"Map\\map1-1_bank.png", D3DCOLOR_XRGB(255, 0, 0));
	textures->Add(ID_TEX_MARIO, L"textures\\mario.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_ENEMY, L"textures\\enemies.png", D3DCOLOR_XRGB(3, 26, 110));
	textures->Add(ID_TEX_ENEMY_2, L"textures\\enemies-2.png", D3DCOLOR_XRGB(0, 128, 128));
	textures->Add(ID_TEX_MISC, L"textures\\misc.png", D3DCOLOR_XRGB(176, 224, 248));

	map = new Map(27, 176, 12, 11, ID_TEX_MAP, L"Map\\map1-1.txt");

	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texMario = textures->Get(ID_TEX_MARIO);

	sprites->Add(10099, 215, 120, 231, 135, texMario);		// die 

	// big
	sprites->Add(10001, 246, 154, 260, 181, texMario);		// idle big right
	sprites->Add(10002, 275, 154, 290, 181, texMario);		// walk
	sprites->Add(10003, 304, 154, 321, 181, texMario);
	sprites->Add(10004, 395, 275, 411, 301, texMario);		// jum big right

	sprites->Add(10011, 186, 154, 200, 181, texMario);		// idle big left
	sprites->Add(10012, 155, 154, 170, 181, texMario);		// walk
	sprites->Add(10013, 125, 154, 140, 181, texMario);
	sprites->Add(10014, 35, 275, 51, 301, texMario);		// jum big left

	// small
	sprites->Add(10021, 247, 0, 259, 15, texMario);			// idle small right
	sprites->Add(10022, 275, 0, 291, 15, texMario);			// walk 
	sprites->Add(10023, 306, 0, 320, 15, texMario);			//
	sprites->Add(10024, 397, 0, 410, 16, texMario);			// jum small right

	sprites->Add(10031, 187, 0, 198, 15, texMario);			// idle small left
	sprites->Add(10032, 155, 0, 170, 15, texMario);			// walk
	sprites->Add(10033, 125, 0, 139, 15, texMario);
	sprites->Add(10034, 36, 0, 49, 16, texMario);			// jum small left

	// raccoon
	sprites->Add(10041, 243, 634, 264, 662, texMario);		// idle raccoon right
	sprites->Add(10042, 272, 634, 293, 662, texMario);		// walk 
	sprites->Add(10043, 303, 634, 324, 662, texMario);		//
	sprites->Add(10044, 303, 714, 324, 741, texMario);		// jum raccoon right

	sprites->Add(10051, 182, 634, 203, 662, texMario);		// idle raccoon left
	sprites->Add(10052, 153, 634, 174, 662, texMario);		// walk
	sprites->Add(10053, 122, 634, 143, 662, texMario);
	sprites->Add(10054, 122, 714, 143, 741, texMario);		// jum raccoon left

	

	LPANIMATION ani;

	ani = new CAnimation(100);		// Mario die
	ani->Add(10099);
	animations->Add(599, ani);



	ani = new CAnimation(100);	// idle big right
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	// idle big left
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(100);	// idle small right
	ani->Add(10021);
	animations->Add(402, ani);

	ani = new CAnimation(100);	// idle small left
	ani->Add(10031);
	animations->Add(403, ani);

	ani = new CAnimation(100);	// idle raccoon right
	ani->Add(10041);
	animations->Add(404, ani);

	ani = new CAnimation(100);	// idle raccoon left
	ani->Add(10051);
	animations->Add(405, ani);



	ani = new CAnimation(100);	// walk big right
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);

	ani = new CAnimation(100);	// walk big left
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(501, ani);

	ani = new CAnimation(50);	// walk small right
	ani->Add(10021);
	ani->Add(10022);
	ani->Add(10023);
	animations->Add(502, ani);

	ani = new CAnimation(50);	// walk small left
	ani->Add(10031);
	ani->Add(10032);
	ani->Add(10033);
	animations->Add(503, ani);

	ani = new CAnimation(100);	// walk raccoon right
	ani->Add(10041);
	ani->Add(10042);
	ani->Add(10043);
	animations->Add(504, ani);

	ani = new CAnimation(100);	// walk raccoon left
	ani->Add(10051);
	ani->Add(10052);
	ani->Add(10053);
	animations->Add(505, ani);
	


	ani = new CAnimation(100);		// jump big right
	ani->Add(10004);
	animations->Add(600, ani);

	ani = new CAnimation(100);		// jump big left
	ani->Add(10014);
	animations->Add(601, ani);

	ani = new CAnimation(100);		// jump small right
	ani->Add(10024);
	animations->Add(602, ani);

	ani = new CAnimation(100);		// jump small left
	ani->Add(10034);
	animations->Add(603, ani);

	ani = new CAnimation(100);		// jump raccoon right
	ani->Add(10044);
	animations->Add(604, ani);

	ani = new CAnimation(100);		// jump raccoon left
	ani->Add(10054);
	animations->Add(605, ani);

	

	mario = new CMario();

	mario->AddAnimation(599);		// die

	mario->AddAnimation(402);		// idle right small
	mario->AddAnimation(403);		// idle left small
	mario->AddAnimation(502);		// walk right small
	mario->AddAnimation(503);		// walk left small
	mario->AddAnimation(602);		// small jump right
	mario->AddAnimation(603);		// small jump left

	mario->AddAnimation(400);		// idle right big
	mario->AddAnimation(401);		// idle left big
	mario->AddAnimation(500);		// walk right big
	mario->AddAnimation(501);		// walk left big
	mario->AddAnimation(600);		// big jump right
	mario->AddAnimation(601);		// big jump left

	mario->AddAnimation(404);		// idle right raccoon
	mario->AddAnimation(405);		// idle left raccoon
	mario->AddAnimation(504);		// walk right raccoon
	mario->AddAnimation(505);		// walk left raccoon
	mario->AddAnimation(604);		// big jump raccoon
	mario->AddAnimation(605);		// big jump raccoon

	mario->SetPosition(0.0f, 388.0f);
	objects.push_back(mario);

	// LOAD SHELL
	LPDIRECT3DTEXTURE9 texEnemy_2 = textures->Get(ID_TEX_ENEMY_2);
	sprites->Add(20001, 239, 202, 255, 218, texEnemy_2);
	sprites->Add(20002, 256, 202, 272, 218, texEnemy_2);
	sprites->Add(20003, 273, 202, 289, 218, texEnemy_2);
	sprites->Add(20004, 290, 202, 306, 218, texEnemy_2);

	ani = new CAnimation(1000);	// shell idle
	ani->Add(20001);
	animations->Add(201, ani);

	ani = new CAnimation(100);	// shell walking
	ani->Add(20001);
	ani->Add(20002);
	ani->Add(20003);
	ani->Add(20004);
	animations->Add(202, ani);

	shell = new CShell();
	shell->AddAnimation(201);	// shell idle
	shell->AddAnimation(202);	// shell walking
	shell->SetPosition(575, 360);
	objects.push_back(shell);

	// LOAD KOOPA
	sprites->Add(70001, 170, 191, 186, 218, texEnemy_2); // koopa walking right
	sprites->Add(70002, 187, 191, 203, 218, texEnemy_2);
	sprites->Add(70003, 205, 191, 221, 218, texEnemy_2); // koopa walking left
	sprites->Add(70004, 222, 191, 238, 218, texEnemy_2);

	ani = new CAnimation(100);	// koopa walking right
	ani->Add(70001);
	ani->Add(70002);
	animations->Add(101, ani);

	ani = new CAnimation(100);	// koopa walking left
	ani->Add(70003);
	ani->Add(70004);
	animations->Add(102, ani);

	koopa = new CKoopa();
	koopa->AddAnimation(101);
	koopa->AddAnimation(102);
	koopa->SetPosition(577, 357);
	koopa->setAactiveArea(512, 607);
	koopa->setMyShell(shell);
	objects.push_back(koopa);

	// LOAD GOOMBA
	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);
	sprites->Add(30001, 5, 14, 21, 29, texEnemy);
	sprites->Add(30002, 25, 14, 41, 29, texEnemy);

	sprites->Add(30003, 45, 21, 61, 29, texEnemy); // die sprite

	ani = new CAnimation(300);		// Goomba walk
	ani->Add(30001);
	ani->Add(30002);
	animations->Add(701, ani);

	ani = new CAnimation(1000);		// Goomba dead
	ani->Add(30003);
	animations->Add(702, ani);

	goomba = new CGoomba();
	goomba->AddAnimation(701);
	goomba->AddAnimation(702);
	goomba->SetPosition(224, 400);
	goomba->SetState(GOOMBA_STATE_WALKING);
	goomba->SetActiveArea(-30, 352);
	objects.push_back(goomba);

	goomba = new CGoomba();
	goomba->AddAnimation(701);
	goomba->AddAnimation(702);
	goomba->SetPosition(528, 400);
	goomba->SetState(GOOMBA_STATE_WALKING);
	goomba->SetActiveArea(384, 623);
	objects.push_back(goomba);

	goomba = new CGoomba();
	goomba->AddAnimation(701);
	goomba->AddAnimation(702);
	goomba->SetPosition(832, 384);
	goomba->SetState(GOOMBA_STATE_WALKING);
	goomba->SetActiveArea(672, 1087);
	objects.push_back(goomba);

	goomba = new CGoomba();
	goomba->AddAnimation(701);
	goomba->AddAnimation(702);
	goomba->SetPosition(880, 384);
	goomba->SetState(GOOMBA_STATE_WALKING);
	goomba->SetActiveArea(672, 1087);
	objects.push_back(goomba);

	// LOAD COIN
	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_MISC);

	//sprites->Add(50001, 303, 99, 313, 115, texMisc);
	//sprites->Add(50002, 321, 99, 331, 115, texMisc);
	//sprites->Add(50003, 339, 99, 349, 115, texMisc);

	//ani = new CAnimation(100);		// Coin idle
	//ani->Add(50001);
	//ani->Add(50002);
	//ani->Add(50003);
	//animations->Add(800, ani);

	//for (int i = 0; i < 28; i++) {
	//	coin = new CCoin();
	//	coin->AddAnimation(800);
	//	coins.push_back(coin);
	//}

	//coins[5]->SetPosition(1073, 320);
	//coins[6]->SetPosition(1105, 288);
	//coins[7]->SetPosition(1137, 256);
	//coins[8]->SetPosition(1169, 224);
	//coins[9]->SetPosition(1201, 192);

	//coins[10]->SetPosition(1329, 80);
	//coins[11]->SetPosition(1345, 80);
	//coins[12]->SetPosition(1361, 80);

	//coins[13]->SetPosition(1473, 112);
	//coins[14]->SetPosition(1489, 112);

	//coins[15]->SetPosition(1537, 128);
	//coins[16]->SetPosition(1553, 128);
	//coins[17]->SetPosition(1569, 128);
	//coins[18]->SetPosition(1585, 128);

	//coins[19]->SetPosition(1617, 112);
	//coins[20]->SetPosition(1633, 112);
	//coins[21]->SetPosition(1649, 112);
	//coins[22]->SetPosition(1665, 112);

	//coins[23]->SetPosition(1697, 144);
	//coins[24]->SetPosition(1729, 112);
	//coins[25]->SetPosition(1761, 128);
	//coins[26]->SetPosition(1809, 96);
	//coins[27]->SetPosition(1841, 128);

	// LOAD SUPERMUSHROOM
	//sprites->Add(40001, 300, 189, 316, 205, texMisc);
	//ani = new CAnimation(100);		// SuperMushroom idle
	//ani->Add(40001);
	//animations->Add(300, ani);

	//for (int i = 0; i < 3; i++) {
	//	superMushroom = new CSuperMushroom();
	//	superMushroom->AddAnimation(300);
	//	superMushrooms.push_back(superMushroom);
	//	objects.push_back(superMushroom);
	//}

	// LOAD BRICKREWARD
	sprites->Add(60001, 300, 117, 316, 133, texMisc);
	sprites->Add(60002, 318, 117, 334, 133, texMisc);
	sprites->Add(60003, 336, 117, 352, 133, texMisc);
	sprites->Add(60004, 354, 117, 370, 133, texMisc);
	sprites->Add(60005, 372, 117, 388, 133, texMisc);

	ani = new CAnimation(100);		// BrickReward idle
	ani->Add(60001);
	ani->Add(60002);
	ani->Add(60003);
	ani->Add(60004);
	animations->Add(900, ani);

	ani = new CAnimation(1000);		// BrickReward actived
	ani->Add(60005);
	animations->Add(901, ani);

	brickReward = new CBrickReward();
	brickReward->AddAnimation(900);
	brickReward->AddAnimation(901);
	brickReward->SetPosition(176, 352);
	//brickReward->setReward(coins[0]);
	brickRewards.push_back(brickReward);

	brickReward = new CBrickReward();
	brickReward->AddAnimation(900);
	brickReward->AddAnimation(901);
	brickReward->SetPosition(192, 352);
	//brickReward->setReward(coins[1]);
	brickRewards.push_back(brickReward);

	brickReward = new CBrickReward();
	brickReward->AddAnimation(900);
	brickReward->AddAnimation(901);
	brickReward->SetPosition(224, 304);
	//brickReward->setReward(coins[2]);
	brickRewards.push_back(brickReward);

	brickReward = new CBrickReward();
	brickReward->AddAnimation(900);
	brickReward->AddAnimation(901);
	brickReward->SetPosition(240, 304);
	//brickReward->setReward(superMushrooms[1]);
	brickRewards.push_back(brickReward);

	brickReward = new CBrickReward();
	brickReward->AddAnimation(900);
	brickReward->AddAnimation(901);
	brickReward->SetPosition(416, 320);
	//brickReward->setReward(coins[3]);
	brickRewards.push_back(brickReward);

	brickReward = new CBrickReward();
	brickReward->AddAnimation(900);
	brickReward->AddAnimation(901);
	brickReward->SetPosition(656, 384);
	//brickReward->setReward(superMushrooms[2]);
	brickRewards.push_back(brickReward);

	brickReward = new CBrickReward();
	brickReward->AddAnimation(900);
	brickReward->AddAnimation(901);
	brickReward->SetPosition(704, 352);
	//brickReward->setReward(coins[4]);
	brickRewards.push_back(brickReward);

	brickReward = new CBrickReward();
	brickReward->AddAnimation(900);
	brickReward->AddAnimation(901);
	brickReward->SetPosition(1440, 128);
	brickRewards.push_back(brickReward);

	brickReward = new CBrickReward();
	brickReward->AddAnimation(900);
	brickReward->AddAnimation(901);
	brickReward->SetPosition(1472, 368);
	//brickReward->setReward(superMushrooms[0]);
	brickRewards.push_back(brickReward);

	// LOAD BLOCK
	LPDIRECT3DTEXTURE9 texMap = textures->Get(ID_TEX_MAP);
	// LOAD BLOCK GROUND
	sprites->Add(0, 96, 160, 112, 176, texMap);
	sprites->Add(1, 112, 160, 128, 176, texMap);
	sprites->Add(2, 128, 160, 144, 176, texMap);

	//----------

	block = new CBlock(0);
	block->SetPosition(0, 416);
	blocks.push_back(block);

	block = new CBlock(0);
	block->SetPosition(624, 400);
	blocks.push_back(block);

	block = new CBlock(0);
	block->SetPosition(1152, 416);
	blocks.push_back(block);

	block = new CBlock(0);
	block->SetPosition(1536, 416);
	blocks.push_back(block);

	block = new CBlock(0);
	block->SetPosition(1664, 416);
	blocks.push_back(block);

	block = new CBlock(0);
	block->SetPosition(2256, 416);
	blocks.push_back(block);

	//----------

	for (int i = 0; i < 37; i++) {
		block = new CBlock(1);
		block->SetPosition(16 + i * 16, 416);
		blocks.push_back(block);
	}

	for (int i = 0; i < 27; i++) {
		block = new CBlock(1);
		block->SetPosition(640 + i * 16, 400);
		blocks.push_back(block);
	}

	for (int i = 0; i < 20; i++) {
		block = new CBlock(1);
		block->SetPosition(1168 + i * 16, 416);
		blocks.push_back(block);
	}

	for (int i = 0; i < 3; i++) {
		block = new CBlock(1);
		block->SetPosition(1552 + i * 16, 416);
		blocks.push_back(block);
	}

	for (int i = 0; i < 34; i++) {
		block = new CBlock(1);
		block->SetPosition(1680 + i * 16, 416);
		blocks.push_back(block);
	}

	for (int i = 0; i < 33; i++) {
		block = new CBlock(1);
		block->SetPosition(2272 + i * 16, 416);
		blocks.push_back(block);
	}

	//----------

	block = new CBlock(2);
	block->SetPosition(608, 416);
	blocks.push_back(block);

	block = new CBlock(2);
	block->SetPosition(1072, 400);
	blocks.push_back(block);

	block = new CBlock(2);
	block->SetPosition(1488, 416);
	blocks.push_back(block);

	block = new CBlock(2);
	block->SetPosition(1600, 416);
	blocks.push_back(block);

	block = new CBlock(2);
	block->SetPosition(2224, 416);
	blocks.push_back(block);

	block = new CBlock(2);
	block->SetPosition(2800, 416);
	blocks.push_back(block);

	// LOAD BLOCK BOX
	sprites->Add(3, 16, 64, 32, 80, texMap);

	block = new CBlock(3);
	block->SetPosition(1600, 368);
	blocks.push_back(block);

	block = new CBlock(3);
	block->SetPosition(1664, 368);
	blocks.push_back(block);

	for (int i = 0; i < 2; i++) {
		block = new CBlock(3);
		block->SetPosition(1504 + i * 16, 336);
		blocks.push_back(block);
	}

	for (int i = 0; i < 2; i++) {
		block = new CBlock(3);
		block->SetPosition(1584 + i * 16, 384);
		blocks.push_back(block);
	}

	for (int i = 0; i < 2; i++) {
		block = new CBlock(3);
		block->SetPosition(1664 + i * 16, 384);
		blocks.push_back(block);
	}

	for (int i = 0; i < 2; i++) {
		block = new CBlock(3);
		block->SetPosition(2256 + i * 16, 304);
		blocks.push_back(block);
	}

	for (int i = 0; i < 2; i++) {
		block = new CBlock(3);
		block->SetPosition(2256 + i * 16, 368);
		blocks.push_back(block);
	}

	for (int i = 0; i < 3; i++) {
		block = new CBlock(3);
		block->SetPosition(1568 + i * 16, 400);
		blocks.push_back(block);
	}

	for (int i = 0; i < 3; i++) {
		block = new CBlock(3);
		block->SetPosition(1664 + i * 16, 400);
		blocks.push_back(block);
	}

	// LOAD BLOCK PIPE
	sprites->Add(4, 112, 0, 128, 16, texMap);
	sprites->Add(5, 128, 0, 144, 16, texMap);
	sprites->Add(6, 160, 0, 176, 16, texMap);
	sprites->Add(7, 0, 16, 16, 32, texMap);

	//----------

	block = new CBlock(4);
	block->SetPosition(352, 368);
	blocks.push_back(block);

	block = new CBlock(5);
	block->SetPosition(368, 368);
	blocks.push_back(block);

	block = new CBlock(4);
	block->SetPosition(1792, 384);
	blocks.push_back(block);

	block = new CBlock(5);
	block->SetPosition(1808, 384);
	blocks.push_back(block);

	block = new CBlock(4);
	block->SetPosition(1856, 368);
	blocks.push_back(block);

	block = new CBlock(5);
	block->SetPosition(1872, 368);
	blocks.push_back(block);

	block = new CBlock(4);
	block->SetPosition(2256, 112);
	blocks.push_back(block);

	block = new CBlock(5);
	block->SetPosition(2272, 112);
	blocks.push_back(block);

	block = new CBlock(4);
	block->SetPosition(2320, 384);
	blocks.push_back(block);

	block = new CBlock(5);
	block->SetPosition(2336, 384);
	blocks.push_back(block);

	//----------

	for (int i = 0; i < 2; i++) {
		block = new CBlock(6);
		block->SetPosition(352, 384 + i * 16);
		blocks.push_back(block);
	}

	for (int i = 0; i < 1; i++) {
		block = new CBlock(6);
		block->SetPosition(1792, 400 + i * 16);
		blocks.push_back(block);
	}

	for (int i = 0; i < 2; i++) {
		block = new CBlock(6);
		block->SetPosition(1856, 384 + i * 16);
		blocks.push_back(block);
	}

	for (int i = 0; i < 11; i++) {
		block = new CBlock(6);
		block->SetPosition(2256, 128 + i * 16);
		blocks.push_back(block);
	}

	for (int i = 0; i < 2; i++) {
		block = new CBlock(6);
		block->SetPosition(2256, 384 + i * 16);
		blocks.push_back(block);
	}

	for (int i = 0; i < 1; i++) {
		block = new CBlock(6);
		block->SetPosition(2320, 400 + i * 16);
		blocks.push_back(block);
	}

	//----------

	for (int i = 0; i < 2; i++) {
		block = new CBlock(7);
		block->SetPosition(368, 384 + i * 16);
		blocks.push_back(block);
	}

	for (int i = 0; i < 1; i++) {
		block = new CBlock(7);
		block->SetPosition(1808, 400 + i * 16);
		blocks.push_back(block);
	}

	for (int i = 0; i < 2; i++) {
		block = new CBlock(7);
		block->SetPosition(1872, 384 + i * 16);
		blocks.push_back(block);
	}

	for (int i = 0; i < 11; i++) {
		block = new CBlock(7);
		block->SetPosition(2272, 128 + i * 16);
		blocks.push_back(block);
	}

	for (int i = 0; i < 2; i++) {
		block = new CBlock(7);
		block->SetPosition(2272, 384 + i * 16);
		blocks.push_back(block);
	}

	for (int i = 0; i < 1; i++) {
		block = new CBlock(7);
		block->SetPosition(2336, 400 + i * 16);
		blocks.push_back(block);
	}

	// LOAD BLOCK CLOUD
	sprites->Add(8, 16, 32, 32, 48, texMap);

	//----------

	for (int i = 0; i < 4; i++) {
		block = new CBlock(8, 1);
		block->SetPosition(1408 + i * 16, 192);
		blocks.push_back(block);
	}
	for (int i = 0; i < 13; i++) {
		block = new CBlock(8, 1);
		block->SetPosition(1488 + i * 16, 176);
		blocks.push_back(block);
	}

	// LOAD BLOCK 1 SIDE
	// LOAD BLOCK GREEN
	sprites->Add(9, 128, 32, 144, 48, texMap);
	sprites->Add(10, 144, 32, 160, 48, texMap);
	sprites->Add(11, 160, 32, 176, 48, texMap);

	//----------
	block = new CBlock(9, 1);
	block->SetPosition(400, 368);
	blocks.push_back(block);
	for (int i = 0; i < 3; i++) {
		block = new CBlock(10, 1);
		block->SetPosition(416 + i * 16, 368);
		blocks.push_back(block);
	}
	block = new CBlock(11, 1);
	block->SetPosition(464, 368);
	blocks.push_back(block);

	//----------
	block = new CBlock(9, 1);
	block->SetPosition(512, 384);
	blocks.push_back(block);
	for (int i = 0; i < 4; i++) {
		block = new CBlock(10, 1);
		block->SetPosition(528 + i * 16, 384);
		blocks.push_back(block);
	}
	block = new CBlock(11, 1);
	block->SetPosition(592, 384);
	blocks.push_back(block);

	//----------
	block = new CBlock(9, 1);
	block->SetPosition(1264, 384);
	blocks.push_back(block);
	for (int i = 0; i < 5; i++) {
		block = new CBlock(10, 1);
		block->SetPosition(1280 + i * 16, 384);
		blocks.push_back(block);
	}
	block = new CBlock(11, 1);
	block->SetPosition(1360, 384);
	blocks.push_back(block);

	//----------
	block = new CBlock(9, 1);
	block->SetPosition(2176, 272);
	blocks.push_back(block);
	for (int i = 0; i < 1; i++) {
		block = new CBlock(10, 1);
		block->SetPosition(2192 + i * 16, 272);
		blocks.push_back(block);
	}
	block = new CBlock(11, 1);
	block->SetPosition(2208, 272);
	blocks.push_back(block);

	// LOAD BLOCK PINK
	sprites->Add(12, 16, 16, 32, 32, texMap);
	sprites->Add(13, 32, 16, 48, 32, texMap);
	sprites->Add(14, 48, 16, 64, 32, texMap);

	//----------
	block = new CBlock(12, 1);
	block->SetPosition(240, 368);
	blocks.push_back(block);
	for (int i = 0; i < 1; i++) {
		block = new CBlock(13, 1);
		block->SetPosition(256 + i * 16, 368);
		blocks.push_back(block);
	}
	block = new CBlock(14, 1);
	block->SetPosition(272, 368);
	blocks.push_back(block);

	//----------
	block = new CBlock(12, 1);
	block->SetPosition(464, 336);
	blocks.push_back(block);
	for (int i = 0; i < 2; i++) {
		block = new CBlock(13, 1);
		block->SetPosition(480 + i * 16, 336);
		blocks.push_back(block);
	}
	block = new CBlock(14, 1);
	block->SetPosition(512, 336);
	blocks.push_back(block);

	//----------
	block = new CBlock(12, 1);
	block->SetPosition(1296, 352);
	blocks.push_back(block);
	for (int i = 0; i < 5; i++) {
		block = new CBlock(13, 1);
		block->SetPosition(1312 + i * 16, 352);
		blocks.push_back(block);
	}
	block = new CBlock(14, 1);
	block->SetPosition(1392, 352);
	blocks.push_back(block);

	//----------
	block = new CBlock(12, 1);
	block->SetPosition(1312, 144);
	blocks.push_back(block);
	for (int i = 0; i < 2; i++) {
		block = new CBlock(13, 1);
		block->SetPosition(1328 + i * 16, 144);
		blocks.push_back(block);
	}
	block = new CBlock(14, 1);
	block->SetPosition(1360, 144);
	blocks.push_back(block);

	//----------
	block = new CBlock(12, 1);
	block->SetPosition(2144, 368);
	blocks.push_back(block);
	for (int i = 0; i < 2; i++) {
		block = new CBlock(13, 1);
		block->SetPosition(2160 + i * 16, 368);
		blocks.push_back(block);
	}
	block = new CBlock(14, 1);
	block->SetPosition(2176, 368);
	blocks.push_back(block);

	// LOAD BLOCK BLUE
	sprites->Add(15, 112, 64, 128, 80, texMap);
	sprites->Add(16, 128, 64, 144, 80, texMap);
	sprites->Add(17, 144, 64, 160, 80, texMap);

	//----------
	block = new CBlock(15, 1);
	block->SetPosition(272, 336);
	blocks.push_back(block);
	for (int i = 0; i < 1; i++) {
		block = new CBlock(16, 1);
		block->SetPosition(288 + i * 16, 336);
		blocks.push_back(block);
	}
	block = new CBlock(17, 1);
	block->SetPosition(304, 336);
	blocks.push_back(block);

	//----------
	block = new CBlock(15, 1);
	block->SetPosition(1328, 320);
	blocks.push_back(block);
	for (int i = 0; i < 5; i++) {
		block = new CBlock(16, 1);
		block->SetPosition(1344 + i * 16, 320);
		blocks.push_back(block);
	}
	block = new CBlock(17, 1);
	block->SetPosition(1424, 320);
	blocks.push_back(block);

	// LOAD BLOCK WHITE
	sprites->Add(18, 128, 48, 144, 64, texMap);
	sprites->Add(19, 144, 48, 160, 64, texMap);
	sprites->Add(20, 160, 48, 176, 64, texMap);

	//----------
	block = new CBlock(18, 1);
	block->SetPosition(512, 304);
	blocks.push_back(block);
	for (int i = 0; i < 2; i++) {
		block = new CBlock(19, 1);
		block->SetPosition(528 + i * 16, 304);
		blocks.push_back(block);
	}
	block = new CBlock(20, 1);
	block->SetPosition(560, 304);
	blocks.push_back(block);
}

void Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		if (!objects[i]->isDisable)
			coObjects.push_back(objects[i]);
	}
	for (int i = 0; i < blocks.size(); i++)
	{
		coObjects.push_back(blocks[i]);
	}
	for (int i = 0; i < brickRewards.size(); i++) {
		coObjects.push_back(brickRewards[i]);
	}
	/*for (int i = 0; i < coins.size(); i++) {
		if (!coins[i]->isDisable)
			coObjects.push_back(coins[i]);
	}


	for (int i = 0; i < coins.size(); i++) {
		coins[i]->Update(dt, &coObjects);
	}*/
	for (int i = 0; i < brickRewards.size(); i++) {
		brickRewards[i]->Update(dt);
	}
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}


	// Set camera position
	float cx, cy;
	mario->GetPosition(cx, cy);
	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;
	if (cy < 0) cy = 0;
	if (cy > map->getMapHeight() - game->GetScreenHeight()) cy = map->getMapHeight() - game->GetScreenHeight();
	if (cx < 0) cx = 0;
	if (cx > map->getMapWidth() - game->GetScreenWidth()) cx = map->getMapWidth() - game->GetScreenWidth();
	CGame::GetInstance()->SetCamPos(cx, cy);
}

/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		map->Render();

		for (int i = 0; i < blocks.size(); i++) {
			blocks[i]->Render();
		}
		/*for (int i = 0; i < coins.size(); i++) {
			if(!coins[i]->isDisable)
				coins[i]->Render();
		}*/
		for (int i = 0; i < brickRewards.size(); i++) {
			brickRewards[i]->Render();
		}
		for (int i = 0; i < objects.size(); i++) {
			if (!objects[i]->isDisable)
				objects[i]->Render();
		}

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}