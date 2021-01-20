#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
//#include "Portal.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_MAP				3
#define SCENE_SECTION_ZONE				4
#define SCENE_SECTION_HUD				5
#define SCENE_SECTION_SPRITES			6
#define SCENE_SECTION_ANIMATIONS		7
#define SCENE_SECTION_ANIMATION_SETS	8
#define SCENE_SECTION_OBJECTS			9

#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_BLOCK				1
#define OBJECT_TYPE_GOOMBA				2
#define OBJECT_TYPE_KOOPA				3
#define OBJECT_TYPE_PARAGOOMBA			4
#define OBJECT_TYPE_PARAKOOPA			5
#define OBJECT_TYPE_COIN				6
#define OBJECT_TYPE_BREAKBLOCK			7
#define OBJECT_TYPE_SWITCHBLOCK			8
#define OBJECT_TYPE_BRICKREWARD			9
#define OBJECT_TYPE_SUPERMUSHROOM		10
#define OBJECT_TYPE_SUPERLEAF			11
#define OBJECT_TYPE_GATE				12
#define OBJECT_TYPE_HUD					13
#define OBJECT_TYPE_PIRANHAPLANT		14
#define OBJECT_TYPE_VENUSFIRETRAP		15
#define OBJECT_TYPE_BUSH				16
#define OBJECT_TYPE_MARIOWORLDMAP		17
#define OBJECT_TYPE_STATION				18
#define OBJECT_TYPE_LOTTERY				19
#define OBJECT_TYPE_LOGO				20
#define OBJECT_TYPE_MENU				21

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	float PosX = atof(tokens[0].c_str());
	float PosY = atof(tokens[1].c_str());
	int TotalRowsOfMap = atoi(tokens[2].c_str());
	int TotalColumnsOfMap = atoi(tokens[3].c_str());
	int TotalRowsOfTileSet = atoi(tokens[4].c_str());
	int TotalColumnsOfTileSet = atoi(tokens[5].c_str());
	int TileSetID = atoi(tokens[6].c_str());
	wstring mapMatrixPath = ToWSTR(tokens[7]);

	this->map = new Map(PosX, PosY,	TotalRowsOfMap, TotalColumnsOfMap, TotalRowsOfTileSet, TotalColumnsOfTileSet, TileSetID, mapMatrixPath);
}

void CPlayScene::_ParseSection_ZONE(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	float left = atof(tokens[1].c_str());
	float top = atof(tokens[2].c_str());
	float right = atof(tokens[3].c_str());
	float bottom = atof(tokens[4].c_str());

	CZone* zone = new CZone(id, left, top, right, bottom);
	zones.push_back(zone);
}

void CPlayScene::_ParseSection_HUD(string line)
{
	vector<string> tokens = split(line);

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	if (tokens.size() < 3) return; // skip invalid lines

	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int ani_id = atoi(tokens[0].c_str());

	hud = new CHUD();
	hud->SetPosition(x, y);
	hud->SetDefaultPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(15);

	hud->SetAnimationSet(ani_set);

	if (hud)
		DebugOut(L"[INFO] HUD created!\n");
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario();
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_BLOCK:
	{
		int tB = atoi(tokens[4].c_str());
		obj = new CBlock(tB);
		break;
	}
	case OBJECT_TYPE_GOOMBA: 
	{
		obj = new CGoomba();
		break;
	}
	case OBJECT_TYPE_KOOPA:
	{
		int type = atoi(tokens[4].c_str());
		obj = new CKoopa(type);
		break;		
	}
	case OBJECT_TYPE_PARAGOOMBA:
	{
		obj = new CParaGoomba();
		break;
	}
	case OBJECT_TYPE_PARAKOOPA:
	{
		int type = atoi(tokens[4].c_str());
		obj = new CParaKoopa(type);
		break;
	}
	case OBJECT_TYPE_COIN:
	{
		obj = new CCoin();
		break;
	}
	case OBJECT_TYPE_BREAKBLOCK:
	{
		int type = atoi(tokens[4].c_str());
		obj = new CBreakBlock(type);
		break;
	}
	case OBJECT_TYPE_SWITCHBLOCK:
	{
		obj = new CSwitchBlock();
		break;
	}
	case OBJECT_TYPE_BRICKREWARD:
	{
		int type = atoi(tokens[4].c_str());
		obj = new CBrickReward(type);
		break;
	}
	case OBJECT_TYPE_SUPERMUSHROOM:
	{
		int type = atoi(tokens[4].c_str());
		obj = new CSuperMushroom(type);
		break;
	}
	case OBJECT_TYPE_SUPERLEAF:
	{
		obj = new CSuperLeaf();
		break;
	}
	case OBJECT_TYPE_GATE:
	{
		float targetX = atof(tokens[4].c_str());
		float targetY = atof(tokens[5].c_str());
		int targetZone = atoi(tokens[6].c_str());
		int type = atoi(tokens[7].c_str());
		obj = new CGate(targetX, targetY, targetZone, type);
		break;
	}
	case OBJECT_TYPE_HUD:
	{
		obj = new CHUD();
		break;
	}
	case OBJECT_TYPE_PIRANHAPLANT:
	{
		obj = new CPiranhaPlant();
		break;
	}
	case OBJECT_TYPE_VENUSFIRETRAP:
	{
		int type = atoi(tokens[4].c_str());
		obj = new CVenusFireTrap(type);
		break;
	}
	case OBJECT_TYPE_BUSH:
	{
		obj = new CBush();
		break;
	}
	case OBJECT_TYPE_MARIOWORLDMAP:
	{
		obj = new CMarioWorldMap();
		marioWorldMap = (CMarioWorldMap*)obj;

		DebugOut(L"[INFO] Mario World Map object created!\n");
		break;
	}
	case OBJECT_TYPE_STATION:
	{
		bool l = atoi(tokens[4].c_str());
		bool t = atoi(tokens[5].c_str());
		bool r = atoi(tokens[6].c_str());
		bool b = atoi(tokens[7].c_str());
		int sceneId = atoi(tokens[8].c_str());
		obj = new CStation(l, t, r, b, sceneId);
		break;
	}
	case OBJECT_TYPE_LOTTERY:
	{
		obj = new CLottery();
		break;
	}
	case OBJECT_TYPE_LOGO:
	{
		obj = new CLogo();
		break;
	}
	case OBJECT_TYPE_MENU:
	{
		obj = new CMenu();
		introMenu = (CMenu*)obj;

		DebugOut(L"[INFO] Intro Menu object created!\n");
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	obj->SetDefaultPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		
		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line == "[ZONE]") { section = SCENE_SECTION_ZONE; continue; }
		if (line == "[HUD]") { section = SCENE_SECTION_HUD; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_ZONE: _ParseSection_ZONE(line); break;
		case SCENE_SECTION_HUD: _ParseSection_HUD(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	/*CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));*/

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	SwitchZone(currentZone);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (!objects[i]->isDisable)
			coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	UpdateCameraPos();
}

void CPlayScene::UpdateCameraPos() {
	CGame* game = CGame::GetInstance();

	if (hud)
		hud->SetPosition(0,	game->GetScreenHeight() - 40);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	cx -= game->GetScreenWidth() / 2;
	cy -= (game->GetScreenHeight() - 40) / 2;
	if (cx < zoneLeft) cx = zoneLeft;
	if (cx > zoneRight - game->GetScreenWidth()) cx = zoneRight - game->GetScreenWidth();
	if (cy - 40 > zoneBottom - game->GetScreenHeight()) cy = zoneBottom + 40 - game->GetScreenHeight();
	if (cy < zoneTop) cy = zoneTop;

	CGame::GetInstance()->SetCamPos(cx, cy);

	if (hud)
		hud->SetPosition(cx, cy + game->GetScreenHeight() - 40);

	int life = CBackup::GetInstance()->GetLifeStack();
	if (life <= 0) {
		CGame* game = CGame::GetInstance();
		CBackup* backup = CBackup::GetInstance();
		backup->SetLifeStack(4);
		backup->SetCoin(0);
		backup->SetPoint(0);
		game->SwitchScene(2);
		return;
	}
}

void CPlayScene::Render()
{
	if (map)
		map->Render();
	
	/*CGame* game = CGame::GetInstance();
	float l, t, r, b;*/

	for (int i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->isDisable && objects[i]->IsInCamera())
			objects[i]->Render();
	}

	if (hud)
		hud->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];		

	objects.clear();

	for (int i = 0; i < zones.size(); i++)
		delete zones[i];

	zones.clear();

	player = NULL;
	marioWorldMap = nullptr;
	introMenu = nullptr;

	delete map;
	map = nullptr;

	delete hud;
	hud = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CGame* game = CGame::GetInstance();

	if (game->GetCurrentSceneId() == 3) {
		CMario* mario = ((CPlayScene*)scene)->GetPlayer();
		if (!mario) return;
		if (!mario->canControl) return;

		if (mario->state != MARIO_STATE_DIE) {
			switch (KeyCode)
			{
			case DIK_S:
				if (mario->GetLevel() != MARIO_LEVEL_RACCOON) {
					if (mario->canJump) {
						mario->Jump();
						mario->canJump = false;
					}
				}
				else {
					if (mario->canJump && mario->GetRunSpeedStack() != 7) {
						mario->Jump();
						mario->canJump = false;
					}
					else if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
						mario->Fly();
					}
				}
				break;
			case DIK_X:
				mario->LvlUp();
				break;
			case DIK_A:
				mario->Shot();
				mario->Spin();
				mario->isReadyHug = true;
				break;
			}
		}
	}

	if (game->GetCurrentSceneId() == 2) {
		CMarioWorldMap* mario = ((CPlayScene*)scene)->GetMarioWorldMap();
		if (!mario) return;

		if (mario->GetState() == MARIOWORLDMAP_STATE_IDLE) {
			switch (KeyCode)
			{
			case DIK_LEFT:
				if (mario->canLeft) {
					mario->SetState(MARIOWORLDMAP_STATE_WALKING_LEFT);
					mario->SetDefaultLimit();
					mario->SetCanNotControl();
				}
				break;
			case DIK_RIGHT:
				if (mario->canRight) {
					mario->SetState(MARIOWORLDMAP_STATE_WALKING_RIGHT);
					mario->SetDefaultLimit();
					mario->SetCanNotControl();
				}
				break;
			case DIK_UP:
				if (mario->canUp) {
					mario->SetState(MARIOWORLDMAP_STATE_WALKING_TOP);
					mario->SetDefaultLimit();
					mario->SetCanNotControl();
				}
				break;
			case DIK_DOWN:
				if (mario->canDown) {
					mario->SetState(MARIOWORLDMAP_STATE_WALKING_BOTTOM);
					mario->SetDefaultLimit();
					mario->SetCanNotControl();
				}
				break;
			case DIK_A:
				mario->SwitchScene();
				break;
			}
		}
	}

	if (game->GetCurrentSceneId() == 1) {
		CMenu* introMenu = ((CPlayScene*)scene)->GetIntroMenu();
		if (!introMenu) return;

		switch (KeyCode)
		{
		case DIK_A:
			introMenu->PlayOption();
			break;
		case DIK_S:
			introMenu->ChangeOption();
			break;
		}
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CGame* game = CGame::GetInstance();

	if (game->GetCurrentSceneId() == 3) {
		CMario* mario = ((CPlayScene*)scene)->GetPlayer();
		if (!mario) return;
		if (!mario->canControl) return;

		if (mario->state != MARIO_STATE_DIE) {
			switch (KeyCode)
			{
			case DIK_S:
				mario->SetStopFly();
				mario->SetStopFall();
				break;
			case DIK_A:
				mario->StopHug();
				break;
			}
		}
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (game->GetCurrentSceneId() == 3) {
		CMario* mario = ((CPlayScene*)scene)->GetPlayer();
		if (!mario) return;
		if (!mario->canControl) return;

		if (mario->state != MARIO_STATE_DIE) {
			// disable control key when Mario die 
			if (mario->GetState() == MARIO_STATE_DIE) return;
			if (game->IsKeyDown(DIK_RIGHT)) {
				mario->SetState(MARIO_STATE_WALKING_RIGHT);
				if (game->IsKeyDown(DIK_A))
					mario->SetState(MARIO_STATE_RUN_RIGHT);
			}
			else if (game->IsKeyDown(DIK_LEFT)) {
				mario->SetState(MARIO_STATE_WALKING_LEFT);
				if (game->IsKeyDown(DIK_A))
					mario->SetState(MARIO_STATE_RUN_LEFT);
			}
			else
				mario->SetState(MARIO_STATE_IDLE);
		}
	}
}

void CPlayScene::PushBackObj(CGameObject* obj) {
	objects.push_back(obj);
}