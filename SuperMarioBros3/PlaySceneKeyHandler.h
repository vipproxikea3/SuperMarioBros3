#pragma once
#include "Scene.h"

class CPlaySceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	//CPlaySceneKeyHandler(CScene* s) :CSceneKeyHandler(s) {};
};

