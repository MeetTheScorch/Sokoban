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
		GAME = 3,  //Gra
		DISCONNECT = 4
	};

	enum PacketType //Typy wysylanych pakietow
	{
		LEVEL = 0,   //pakiet przesyla nazwe poziomu do zbudowania
		MESSAGE = 1, //pakiet przesyla wiadomosc
		MOVE = 2	 //pakiet przesyla ruch
	};

	enum ConnectionType //Typ polaczenia
	{
		HOST = 0,
		CLIENT = 1
	};

	struct MyPacketData //Struktura pakietow
	{
		std::string type;	//typ pakietu
		std::string message; //wiadomosc
		int moveX, moveY;	//ruch
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
	void disconnect();
	void receivePacket();
	void sendPacket(PacketType);
	void addMessage(const std::string &);
	//
	void showGame();
	void drawGame();
	void drawChat();
	//
	void move(int, int, Player *);
	bool checkColission(int, int, bool, bool);
	bool checkFinish();
	void loadTexture(const std::string &, const std::string &);
	sf::Texture *getTexture(const std::string &);

	//Przeciazony operator do wysylania paczek
	friend sf::Packet &operator<<(sf::Packet &packet, const MyPacketData &data)
	{
		return packet << data.type << data.message << data.moveX << data.moveY;
	}
	//Przeciazony operator do pobierania paczek
	friend sf::Packet &operator>>(sf::Packet &packet, MyPacketData &data)
	{
		return packet >> data.type >> data.message >> data.moveX >> data.moveY;
	}

private:
	GameState state;			   //Obecny stan aplikacji
	ConnectionType connectionType; //Obecny typ polaczenia
	Data::GameMode mode;		   //Tryb gry
	sf::RenderWindow window;	   //Okno aplikacji
	sf::View gameView;			   //Widok na gre
	sf::View menuView;			   //Widok na menu
	sf::View *currentView;		   //Obecny widok
	sf::Font font;				   //Czcionka uzywana w aplikacji

	Menu *menu;		  //Obiekt menu
	Level *level;	 //obiekt poziomu
	Player *player;   //obiekt gracza
	Player *coPlayer; //obiekt drugiego gracza
	Target *target;   //obiekt celu
	Crate *crate;	 //obiekt skrzyni

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

	std::vector<sf::Text> *messages; //kontener na wiadomosci chatu
	std::string inputString;		 //
	sf::Text inputText;				 //
	std::string messageSend;		 //
	int moveX, moveY;				 //
	bool inputActive;				 //
	std::vector<sf::Text> chat;		 //
};