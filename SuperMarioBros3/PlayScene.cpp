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
#define SCENE_SECTION_SPRITES			5
#define SCENE_SECTION_ANIMATIONS		6
#define SCENE_SECTION_ANIMATION_SETS	7
#define SCENE_SECTION_OBJECTS			8

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

	int TotalRowsOfMap = atoi(tokens[0].c_str());
	int TotalColumnsOfMap = atoi(tokens[1].c_str());
	int TotalRowsOfTileSet = atoi(tokens[2].c_str());
	int TotalColumnsOfTileSet = atoi(tokens[3].c_str());
	int TileSetID = atoi(tokens[4].c_str());
	wstring mapMatrixPath = ToWSTR(tokens[5]);

	this->map = new Map(TotalRowsOfMap, TotalColumnsOfMap, TotalRowsOfTileSet, TotalColumnsOfTileSet, TileSetID, mapMatrixPath);
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
		float limitL = atoi(tokens[4].c_str());
		float limitR = atoi(tokens[5].c_str());
		obj = new CGoomba(limitL, limitR);
		break;
	}
	case OBJECT_TYPE_KOOPA:
	{
		float limitL = atoi(tokens[4].c_str());
		float limitR = atoi(tokens[5].c_str());
		obj = new CKoopa(limitL, limitR);
		break;		
	}
	case OBJECT_TYPE_PARAGOOMBA:
	{
		float limitL = atoi(tokens[4].c_str());
		float limitR = atoi(tokens[5].c_str());
		obj = new CParaGoomba(limitL, limitR);
		break;
	}
	case OBJECT_TYPE_PARAKOOPA:
	{
		float limitL = atoi(tokens[4].c_str());
		float limitR = atoi(tokens[5].c_str());
		obj = new CParaKoopa(limitL, limitR);
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
		obj = new CGate(targetX, targetY, targetZone);
		break;
	}
	/*case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;*/
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
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();

	/*cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;
	if (cx < 0) cx = 0;
	if (cx > map->getMapWidth() - game->GetScreenWidth()) cx = map->getMapWidth() - game->GetScreenWidth();
	if (cy < 0) cy = 0;
	if (cy > map->getMapHeight() - game->GetScreenHeight()) cy = map->getMapHeight() - game->GetScreenHeight();*/

	cx -= game->GetScreenWidth() / 2;
	cy -= (game->GetScreenHeight() - 40) / 2;
	if (cx < zoneLeft) cx = zoneLeft;
	if (cx > zoneRight - game->GetScreenWidth()) cx = zoneRight - game->GetScreenWidth();
	if (cy - 40 > zoneBottom - game->GetScreenHeight()) cy = zoneBottom + 40 - game->GetScreenHeight();
	if (cy < zoneTop) cy = zoneTop;

	CGame::GetInstance()->SetCamPos(cx, cy);
}

void CPlayScene::Render()
{
	if (map)
		map->Render();
	for (int i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->isDisable)
			objects[i]->Render();
	}
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

	delete map;
	map = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
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
		case DIK_Z:
			mario->Shot();
			break;
		case DIK_X:
			mario->Fly();
			break;
		case DIK_C:
			mario->isReadyHug = true;
			break;
		case DIK_S:
			mario->Spin();
			break;
		}
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->state != MARIO_STATE_DIE) {
		switch (KeyCode)
		{
		case DIK_X:
			mario->SetStopFly();
			mario->SetStopFall();
			break;
		case DIK_C:
			mario->StopHug();
			break;
		}
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->state != MARIO_STATE_DIE) {
		// disable control key when Mario die 
		if (mario->GetState() == MARIO_STATE_DIE) return;
		if (game->IsKeyDown(DIK_RIGHT)) {
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
			if (game->IsKeyDown(DIK_LSHIFT))
				mario->SetState(MARIO_STATE_RUN_RIGHT);
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			mario->SetState(MARIO_STATE_WALKING_LEFT);
			if (game->IsKeyDown(DIK_LSHIFT))
				mario->SetState(MARIO_STATE_RUN_LEFT);
		}
		else
			mario->SetState(MARIO_STATE_IDLE);
	}
}

void CPlayScene::PushBackObj(CGameObject* obj) {
	objects.push_back(obj);
}