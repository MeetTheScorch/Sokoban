#include "Data.hpp"

Data::Data() {}

Data &Data::getInstance()
{
	static Data instance;
	return instance;
}

Data::~Data()
{
	singleplayerLevels.clear();
	multiplayerLevels.clear();
}

bool Data::loadContent()
{
	singleplayerLevels.insert({"Level 00", "level0"});
	singleplayerLevels.insert({"Level 01", "level1"});
	singleplayerLevels.insert({"Level 02", "level2"});
	singleplayerLevels.insert({"Level 03", "level3"});
	singleplayerLevels.insert({"Level 04", "level4"});
	singleplayerLevels.insert({"Level 05", "level5"});
	singleplayerLevels.insert({"Level 06", "level6"});
	singleplayerLevels.insert({"Level 07", "level7"});
	singleplayerLevels.insert({"Level 08", "level8"});
	singleplayerLevels.insert({"Level 09", "level9"});
	singleplayerLevels.insert({"Level 10", "level10"});

	multiplayerLevels.insert({"Multi 00", "multi0"});

	return true;
}

std::map<std::string, std::string> &Data::getLevels(GameMode mode)
{
	switch (mode)
	{
	case SINGLEPLAYER:
		return this->singleplayerLevels;
		break;
	case MULTIPLAYER:
		return this->multiplayerLevels;
		break;
	default:
		break;
	}
}

bool Data::nextLevel(GameMode mode, const std::string &name)
{
	switch (mode)
	{
	case SINGLEPLAYER:
		return (std::next(singleplayerLevels.find(name)) != singleplayerLevels.end()) ? true : false;
		break;
	case MULTIPLAYER:
		return (std::next(multiplayerLevels.find(name)) != multiplayerLevels.end()) ? true : false;
		break;
	default:
		return false;
		break;
	}
}

std::string Data::getNextLevel(GameMode mode, const std::string &name)
{
	switch (mode)
	{
	case SINGLEPLAYER:
		return std::next(singleplayerLevels.find(name))->first;
		break;
	case MULTIPLAYER:
		return std::next(multiplayerLevels.find(name))->first;
		break;
	default:
		break;
	}
}