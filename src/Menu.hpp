#pragma once

#include "Data.hpp"

class Menu
{
public:
	enum MenuState
	{
		MAIN = 1,
		SINGLE = 2,
		MULTI = 3,
		INGAME = 4,
		FINISH = 5
	};

	Menu(sf::Font &);
	~Menu();

	MenuState getMenuState() { return this->state; }
	void setMenuState(MenuState state) { this->state = state; }
	void setMenuState(int value) { this->state = MenuState(value); }

	bool loadContent();

	std::vector<sf::Text> getMenuText();
	void setMenuTextPosition(int, float, float);
	void setMenuTextFillColor(int, sf::Color);

private:
	MenuState state;
	sf::Font font;
	std::vector<sf::Text> mainMenuText;
	std::vector<sf::Text> singleMenuText;
	std::vector<sf::Text> multiMenuText;
	std::vector<sf::Text> inGameMenuText;
	std::vector<sf::Text> finishMenuText;
};