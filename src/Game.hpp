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
	enum GameState //Stany w jakich znajduje sie aplikacja
	{
		CLOSE = 0, //Zamykanie aplikacji
		START = 1, //Start aplikacji
		MENU = 2,  //Menu
		GAME = 3   //Gra
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
	//
	void menuMain();
	void menuSingle();
	void menuMulti();
	void menuInGame();
	void menuFinish();
	void menuHost();
	void menuJoin();
	void menuLobby();
	//
	void waitForClient();
	bool connectToHost(const std::string &);
	//
	void showGame();
	void drawGame();
	//
	void move(int, int);
	bool checkColission(int, int, bool);
	bool checkFinish();
	bool checkNextLevel();
	void loadTexture(const std::string &, const std::string &);
	sf::Texture *getTexture(const std::string &);
	void sendMessage();
	void recieveMessage();
	void addMessage(const std::string &);

private:
	GameState state;		 //Obecny stan aplikacji
	sf::RenderWindow window; //Okno aplikacji
	sf::View gameView;		 //Widok na gre
	sf::View menuView;		 //Widok na menu
	sf::Font font;			 //Czcionka uzywana w aplikacji

	Menu *menu;		//
	Level *level;   //
	Player *player; //
	Target *target; //
	Crate *crate;   //gry

	Game();
	bool loadContent();
	void buildLevel(const std::string &);

	std::unordered_map<std::string, sf::Texture *> texturesCollector; //Kontener tekstur

	int textureSize;			  //Rozmiar uzywanych tekstur w pikselach
	unsigned page;				  //Zmienna uzywana do wyswietlania ograniczonej liczby poziomow
	std::string currentLevelName; //Nazwa obecnego poziomu

	std::vector<std::vector<sf::Sprite>> sprites;

	std::vector<Player *> players; //Kontener graczy
	std::vector<Target *> targets; //Kontener celow
	std::vector<Crate *> crates;   //Kontener skrzyn

	const unsigned short PORT = 5000; //Port na ktorym przebiega laczenie
	sf::Thread *thread;				  //Watek do czekania na polaczenie
	sf::TcpSocket *socket;			  //
	sf::Mutex globalMutex;			  //
	bool connected;					  //
};