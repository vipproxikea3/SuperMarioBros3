#pragma once
#include "Game.h"
#include "Backup.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mario.h"
#include "Map.h"
#include "Zone.h"
#include "Block.h"
#include "Goomba.h"
#include "Koopa.h"
#include "ParaGoomba.h"
#include "ParaKoopa.h"
#include "Coin.h"
#include "BreakBlock.h"
#include "SwitchBlock.h"
#include "BrickReward.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "Gate.h"
#include "HUD.h"
#include "BreakBlockPiece.h"
#include "PiranhaPlant.h"
#include "VenusFireTrap.h"
#include "Bush.h"
#include "MarioWorldMap.h"
#include "Station.h"
#include "Lottery.h"
#include "Logo.h"
#include "Menu.h"


class CPlayScene : public CScene
{
protected:
	CMario* player;					// A play scene has to have player, right?
	CMarioWorldMap* marioWorldMap;
	CMenu* introMenu;

	vector<LPGAMEOBJECT> objects;
	vector<CZone*> zones;

	int currentZone = 1;
	float zoneLeft;
	float zoneTop;
	float zoneRight;
	float zoneBottom;
	float zoneWidth;
	float zoneHeight;

	Map* map;

	CHUD* hud;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_ZONE(string line);
	void _ParseSection_HUD(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);


public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void SwitchZone(int zone)
	{
		currentZone = zone;
		for (int i = 0; i < zones.size(); i++) {
			if (zones[i]->GetId() == currentZone) {
				zoneLeft = zones[i]->GetLeft();
				zoneTop = zones[i]->GetTop();
				zoneRight = zones[i]->GetRight();
				zoneBottom = zones[i]->GetBottom();
				zoneWidth = zones[i]->GetZoneWidth();
				zoneHeight = zones[i]->GetZoneHeight();
				break;
			}
		}

		if (player) {
			player->SetZone(zoneLeft, zoneRight, zoneBottom);
		}
	}

	void UpdateCameraPos();
	void PushBackObj(CGameObject* obj);
	vector<LPGAMEOBJECT> GetListObj() { return objects; }

	CMario* GetPlayer() { return player; }
	CMarioWorldMap* GetMarioWorldMap() { return marioWorldMap; }
	CMenu* GetIntroMenu() { return introMenu; }

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CSceneKeyHandler(s) {};
};