#include "Block.h"

CBlock::CBlock(int idSprite) {
	this->idSprite = idSprite;
}

void CBlock::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	sprites->Get(idSprite)->Draw(x, y);
}

void CBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BLOCK_BBOX_WIDTH;
	b = y + BLOCK_BBOX_HEIGHT;
}
