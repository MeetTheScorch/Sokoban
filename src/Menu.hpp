#pragma once

#include "Data.hpp"

class Menu
{
public:
	enum MenuState
	{
		MAIN = 1,   //Main menu
		SINGLE = 2, //Menu for singleplayer
		MULTI = 3,  //Menu for multiplayer
		INGAME = 4, //Ingame menu
		FINISH = 5, //Menu after finishing level
		HOST = 6,   //
		JOIN = 7,   //
		LOBBY = 8   //
	};

	Menu(sf::Font &);
	~Menu();

	MenuState getMenuState() { return this->state; }
	void setMenuState(MenuState state) { this->state = state; }
	void setMenuState(int value) { this->state = MenuState(value); }

	bool loadContent();

	std::vector<sf::Text> &getMenuText();
	void setMenuTextPosition(int, float, float);
	void setMenuTextFillColor(int, sf::Color);
	void setMenuTextString(int, const std::string &);

private:
	MenuState state;
	sf::Font font;
	std::vector<sf::Text> mainMenuText;
	std::vector<sf::Text> singleMenuText;
	std::vector<sf::Text> multiMenuText;
	std::vector<sf::Text> inGameMenuText;
	std::vector<sf::Text> finishMenuText;
	std::vector<sf::Text> hostMenuText;
	std::vector<sf::Text> joinMenuText;
	std::vector<sf::Text> lobbyMenuText;

	std::vector<sf::Text> *menuptr;
};