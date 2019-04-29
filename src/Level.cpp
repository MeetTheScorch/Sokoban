#include "Level.hpp"

Level::Level() {}

Level::~Level()
{
	map.clear();
	playersPositions.clear();
	targetsPositions.clear();
	cratesPositions.clear();
}

/*Wczytywanie poziomu o nazwie name z pliku.*/
bool Level::loadContent(const std::string &name)
{
	setLevelName(name);
	setLevelPath(Data::getInstance().getLevels()[name]);

	std::fstream file;
	file.open("data/" + path);

	if (!file.is_open() || file.eof())
	{
		return false;
	}

	file >> this->sizeX >> this->sizeY;

	if (sizeX == 0 || sizeY == 0)
	{
		file.close();
		return false;
	}

	map.resize(sizeY);
	for (auto y = 0; y < sizeY; y++)
	{
		map[y].resize(sizeX, 2);
	}

	for (auto y = 0; y < sizeY; y++)
	{
		for (auto x = 0; x < sizeX; x++)
		{
			int value;
			file >> value;
			switch (value)
			{
			case 0:
				map[y][x] = value;
				break;
			case 1:
				map[y][x] = value;
				break;
			case 2:
				break;
			case 3:
				targetsPositions.push_back(std::make_pair(x, y));
				break;
			case 4:
				cratesPositions.push_back(std::make_pair(x, y));
				break;
			case 5:
				playersPositions.push_back(std::make_pair(x, y));
				break;
			}
		}
	}

	file.close();
	return true;
}