#pragma once
#include "GameObject.h"

class CZone
{
	int id;
	float left, top, right, bottom;
public:
	CZone(int idex, float l, float t, float r, float b)
	{
		id = idex;
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
	~CZone() {}
	int GetId() { return id; }
	float GetLeft() { return left; }
	float GetTop() { return top; }
	float GetRight() { return right; }
	float GetBottom() { return bottom; }
	float GetZoneWidth() { return right - left; }
	float GetZoneHeight() { return bottom - top; }
};
