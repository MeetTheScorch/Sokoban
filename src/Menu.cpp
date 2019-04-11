#include "Menu.hpp"

Menu::Menu(sf::Font &font)
{
	this->font = font;
	state = MenuState::MAIN;
}

Menu::~Menu() {}

bool Menu::loadContent()
{
	std::vector<sf::String> mainText = {"Main menu", "Single-player", "Multi-player", "Quit"};
	for (auto i = 0u; i < mainText.size(); i++)
	{
		sf::Text t;
		t.setFont(font);
		if (i == 0)
		{
			t.setCharacterSize(50);
		}
		else
		{
			t.setCharacterSize(30);
		}
		t.setString(mainText[i]);
		mainMenuText.push_back(t);
	}

	std::vector<sf::String> singleText = {"Choose level", "Previous 5", "Next 5"};
	for (auto i = 0u; i < singleText.size(); i++)
	{
		sf::Text t;
		t.setFont(font);
		if (i == 0)
		{
			t.setCharacterSize(50);
		}
		else
		{
			t.setCharacterSize(30);
		}
		t.setString(singleText[i]);
		singleMenuText.push_back(t);
	}

	for (auto l : Data::getInstance().getLevels())
	{
		sf::Text t;
		t.setFont(font);
		t.setCharacterSize(25);
		t.setString(l.first);
		singleMenuText.push_back(t);
	}

	std::vector<sf::String> inGameText = {"Back", "Restart", "Menu"};
	for (auto i = 0u; i < inGameText.size(); i++)
	{
		sf::Text t;
		t.setFont(font);
		t.setCharacterSize(30);
		t.setString(inGameText[i]);
		inGameMenuText.push_back(t);
	}

	std::vector<sf::String> finishText = {"Completed!", "Next", "Menu"};
	for (auto i = 0u; i < finishText.size(); i++)
	{
		sf::Text t;
		t.setFont(font);
		if (i == 0)
		{
			t.setCharacterSize(50);
		}
		else
		{
			t.setCharacterSize(30);
		}
		t.setString(finishText[i]);
		finishMenuText.push_back(t);
	}

	return true;
}

std::vector<sf::Text> Menu::getMenuText()
{
	switch (state)
	{
	case MenuState::MAIN:
		return this->mainMenuText;
		break;
	case MenuState::SINGLE:
		return this->singleMenuText;
		break;
	case MenuState::MULTI:
		return this->singleMenuText;
		break;
	case MenuState::INGAME:
		return this->inGameMenuText;
		break;
	case MenuState::FINISH:
		return this->finishMenuText;
		break;
	}
}

void Menu::setMenuTextPosition(int i, float x, float y)
{
	switch (state)
	{
	case MenuState::MAIN:
		this->mainMenuText[i].setPosition(x, y);
		break;
	case MenuState::SINGLE:
		this->singleMenuText[i].setPosition(x, y);
		break;
	case MenuState::MULTI:
		this->singleMenuText[i].setPosition(x, y);
		break;
	case MenuState::INGAME:
		this->inGameMenuText[i].setPosition(x, y);
		break;
	case MenuState::FINISH:
		this->finishMenuText[i].setPosition(x, y);
		break;
	}
}

void Menu::setMenuTextFillColor(int i, sf::Color color)
{
	switch (state)
	{
	case MenuState::MAIN:
		this->mainMenuText[i].setFillColor(color);
		break;
	case MenuState::SINGLE:
		this->singleMenuText[i].setFillColor(color);
		break;
	case MenuState::MULTI:
		this->singleMenuText[i].setFillColor(color);
		break;
	case MenuState::INGAME:
		this->inGameMenuText[i].setFillColor(color);
		break;
	case MenuState::FINISH:
		this->finishMenuText[i].setFillColor(color);
		break;
	}
}