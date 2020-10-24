#include "Map.h"
#include <fstream>
#include "Textures.h"
#include "Game.h"

Map::Map(int TotalRowsOfMap, int TotalColumnsOfMap, int TotalRowsOfTileSet, int TotalColumnsOfTileSet, int TileSetID, LPCWSTR mapMatrixPath) {
	this->TotalRowsOfTileSet = TotalRowsOfTileSet;
	this->TotalColumnsOfTileSet = TotalColumnsOfTileSet;
	this->TotalRowsOfMap = TotalRowsOfMap;
	this->TotalColumnsOfMap = TotalColumnsOfMap;
	TileSetImg = CTextures::GetInstance()->Get(TileSetID);
	LoadMapMatrix(mapMatrixPath);
}

void Map::LoadMapMatrix(LPCWSTR path) {
	fstream inputfile;
	inputfile.open(path, ios::in);
	int tmp;

	for (int i = 0; i < TotalRowsOfMap * TotalColumnsOfMap; i++) {
		inputfile >> tmp;
		MapData.push_back(tmp);
	}

	inputfile.close();
}

void Map::DrawTile(int idOfTile, float x, float y) {
	float xOfTile, yOfTile;
	int colOfTile, rowOfTile;

	rowOfTile = (idOfTile - 1) / TotalColumnsOfTileSet;
	colOfTile = (idOfTile - 1) % TotalColumnsOfTileSet;

	xOfTile = colOfTile * TILE_WIDTH;
	yOfTile = rowOfTile * TILE_HEIGHT;

	CGame::GetInstance()->Draw(x, y, TileSetImg, xOfTile, yOfTile, xOfTile + TILE_WIDTH, yOfTile + TILE_HEIGHT);
}

void Map::Render() {
	float xOfTile, yOfTile;
	int colOfTile, rowOfTile;

	for (int i = 0; i < TotalRowsOfMap * TotalColumnsOfMap; i++) {
		rowOfTile = i / TotalColumnsOfMap;
		colOfTile = i % TotalColumnsOfMap;

		xOfTile = colOfTile * TILE_WIDTH;
		yOfTile = rowOfTile * TILE_HEIGHT;

		DrawTile(MapData[i], xOfTile, yOfTile);
	}
}