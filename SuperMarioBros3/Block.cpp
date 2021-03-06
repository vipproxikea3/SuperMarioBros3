#include "Block.h"

CBlock::CBlock(int typeBlock) {
	this->typeBlock = typeBlock;
	SetBlockSideByTypeBlock(this->typeBlock);
}

void CBlock::SetBlockSide(bool bL, bool bT, bool bR, bool bB) {
	this->bL = bL;
	this->bT = bT;
	this->bR = bR;
	this->bB = bB;
}

void CBlock::SetBlockSideByTypeBlock(int typeBlock) {
	switch (typeBlock)
	{
	case 0:
		SetBlockSide(1, 1, 0, 1);
		break;
	case 1:
		SetBlockSide(0, 1, 0, 1);
		break;
	case 2:
		SetBlockSide(0, 1, 1, 1);
		break;
	case 3:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 4:
		SetBlockSide(1, 1, 0, 0);
		break;
	case 5:
		SetBlockSide(0, 1, 1, 0);
		break;
	case 6:
		SetBlockSide(1, 0, 0, 1);
		break;
	case 7:
		SetBlockSide(0, 0, 1, 1);
		break;
	case 8:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 9:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 10:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 11:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 12:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 13:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 14:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 15:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 16:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 17:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 18:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 19:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 20:
		SetBlockSide(0, 1, 0, 0);
		break;
	case 21:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 22:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 23:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 24:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 25:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 26:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 27:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 28:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 29:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 30:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 31:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 32:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 33:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 34:
		SetBlockSide(1, 1, 1, 1);
		break;
	case 35:
		SetBlockSide(1, 1, 1, 1);
		break;
	}
}

CBlock::~CBlock() {}

void CBlock::Render()
{
	//CSprites* sprites = CSprites::GetInstance();
	animation_set->at(typeBlock)->Render(x, y, 255);
}

void CBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BLOCK_BBOX_WIDTH;
	b = y + BLOCK_BBOX_HEIGHT;
}
