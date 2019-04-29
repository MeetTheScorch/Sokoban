#pragma once

#include "Data.hpp"

class Level
{
  public:
	Level();
	~Level();

	bool loadContent(const std::string &);

	int getSizeX() { return this->sizeX; }
	int getSizeY() { return this->sizeY; }

	void setLevelName(const std::string &name) { this->name = name; }
	std::string getLevelName() { return this->name; }
	void setLevelPath(const std::string &path) { this->path = path; }
	std::string getLevelPath() { return this->path; }

	int getMapValue(int x, int y) { return this->map[y][x]; }
	std::vector<std::pair<int, int>> getPlayersPositions() { return this->playersPositions; }
	std::vector<std::pair<int, int>> getTargetsPositions() { return this->targetsPositions; }
	std::vector<std::pair<int, int>> getCratesPositions() { return this->cratesPositions; }

  private:
	int sizeX, sizeY;	//Rozmiary poziomu
	std::string name;	//Nazwa poziomu
	std::string path;	//Sciezka do poziomu
	std::vector<std::vector<int>> map;
	std::vector<std::pair<int, int>> playersPositions;
	std::vector<std::pair<int, int>> targetsPositions;
	std::vector<std::pair<int, int>> cratesPositions;
};

/*
0 - puste pole
1 - sciana
2 - wolne pole
3 - pole na skrzynie
4 - skrzynia
5 - gracz
*/