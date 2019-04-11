#pragma once

#include "Data.hpp"
#include "Menu.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "Target.hpp"
#include "Crate.hpp"

class Game
{
  public:
	enum GameState
	{
		CLOSE = 0,
		START = 1,
		MENU = 2,
		GAME = 3
	};

	static Game &getInstance();
	~Game();

	GameState getGameState() { return this->state; }
	void setGameState(GameState state) { this->state = state; }
	void setGameState(int value) { this->state = GameState(value); }

	sf::RenderWindow &getWindow() { return this->window; }
	sf::View &getGameView() { return this->gameView; }
	sf::View &getMenuView() { return this->menuView; }

	void run();
	void closeGame();
	void showMenu();
	void menuMain();
	void menuSingle();
	void menuMulti();
	void menuInGame();
	void menuFinish();
	void showGame();
	void drawGame();

	void move(int, int);
	bool checkColission(int, int, bool);
	bool checkFinish();
	bool checkNextLevel();
	void loadTexture(const std::string &, const std::string &);
	sf::Texture *getTexture(const std::string &);

  private:
	GameState state;
	sf::RenderWindow window;
	sf::View gameView;
	sf::View menuView;
	sf::Font font;

	Menu *menu;
	Level *level;
	Player *player;
	Target *target;
	Crate *crate;

	Game();
	bool loadContent();
	void buildLevel(const std::string &);

	std::unordered_map<std::string, sf::Texture *> texturesCollector;

	int textureSize;
	unsigned page;
	std::string currentLevelName;

	std::vector<std::vector<sf::Sprite>> sprites;

	std::vector<Player *> players;
	std::vector<Target *> targets;
	std::vector<Crate *> crates;
};