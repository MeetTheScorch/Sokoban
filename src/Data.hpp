#pragma once

class Data
{
public:
	enum GameMode
	{
		SINGLEPLAYER = 1,
		MULTIPLAYER = 2
	};

	static Data &getInstance();
	~Data();

	bool loadContent();

	std::map<std::string, std::string> &getLevels(GameMode);
	bool nextLevel(GameMode, const std::string &);
	std::string getNextLevel(GameMode, const std::string &);

private:
	Data();
	std::map<std::string, std::string> singleplayerLevels;
	std::map<std::string, std::string> multiplayerLevels;
};