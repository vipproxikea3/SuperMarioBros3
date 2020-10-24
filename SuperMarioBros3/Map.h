#pragma once
#include <d3dx9.h>
#include <vector>
#include <string>

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

using namespace std;

class Map
{
	int TotalRowsOfTileSet, TotalColumnsOfTileSet;
	LPDIRECT3DTEXTURE9 TileSetImg;
	int TotalRowsOfMap, TotalColumnsOfMap;
	std::vector<int> MapData;

public:
	Map(int TotalRowsOfMap, int TotalColumnsOfMap, int TotalRowsOfTileSet, int TotalColumnsOfTileSet, int TileSetID, LPCWSTR mapMatrixPath);
	~Map();
	void LoadMapMatrix(LPCWSTR mapMatrixPath);
	void DrawTile(int idOfTile, float x, float y);
	void Render();
};

