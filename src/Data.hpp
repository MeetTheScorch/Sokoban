#pragma once

class Data
{
public:
	enum LevelsType
	{
		SINGLEPLAYER = 1,
		MULTIPLAYER = 2
	};

	static Data &getInstance();
	~Data();

	bool loadContent();

	std::map<std::string, std::string> &getLevels();
	bool nextLevel(const std::string &);
	std::string getNextLevel(const std::string &);

private:
	Data();
	std::map<std::string, std::string> singleplayerLevels;
	std::map<std::string, std::string> multiplayerLevels;
};