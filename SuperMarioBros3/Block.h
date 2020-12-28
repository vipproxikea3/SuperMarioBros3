#pragma once
#include "GameObject.h"

#define BLOCK_BBOX_WIDTH  16
#define BLOCK_BBOX_HEIGHT 16

class CBlock : public CGameObject
{
	int typeBlock;
	bool bL = true;
	bool bT = true;
	bool bR = true;
	bool bB = true;
public:
	CBlock(int typeBlock);
	~CBlock();
	bool isBlockLeft() { return bL; }
	bool isBlockTop() { return bT; }
	bool isBlockRight() { return bR; }
	bool isBlockBottom() { return bB; }
	void SetBlockSide(bool bL, bool bT, bool bR, bool bB);
	void SetBlockSideByTypeBlock(int typeBlock);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

