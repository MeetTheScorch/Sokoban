#include "Menu.hpp"

Menu::Menu(sf::Font &font)
{
	this->font = font;
	state = MenuState::MAIN;
}

Menu::~Menu() {}

/*Tworzenie tekstu dla kazdego menu.*/
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
	/*Wypelnianie singleMenuText nazwami poziomow.*/
	for (auto l : Data::getInstance().getLevels())
	{
		sf::Text t;
		t.setFont(font);
		t.setCharacterSize(25);
		t.setString(l.first);
		singleMenuText.push_back(t);
	}
	/*Dodawanie na sam koniec singleMenuText tekstu Back.*/
	{
		sf::Text t;
		t.setFont(font);
		t.setCharacterSize(30);
		t.setString("Back");
		singleMenuText.push_back(t);
	}

	std::vector<sf::String> multiText = {"Multi-player", "Host game", "Join game", "Back"};
	for (auto i = 0u; i < multiText.size(); i++)
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
		t.setString(multiText[i]);
		multiMenuText.push_back(t);
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

	std::vector<sf::String> hostText = {"Host game", "Waiting for another player.", "Menu"};
	for (auto i = 0u; i < hostText.size(); i++)
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
		t.setString(hostText[i]);
		hostMenuText.push_back(t);
	}

	std::vector<sf::String> joinText = {"Join game", "Type IP address:", "Menu"};
	for (auto i = 0u; i < joinText.size(); i++)
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
		t.setString(joinText[i]);
		joinMenuText.push_back(t);
	}

	std::vector<sf::String> lobbyText = {"Lobby", "Menu"};
	for (auto i = 0u; i < lobbyText.size(); i++)
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
		t.setString(lobbyText[i]);
		lobbyMenuText.push_back(t);
	}

	return true;
}

/*Funkcja zwraca odpowiednie menu w zaleznosci od zmiennej state.*/
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
		return this->multiMenuText;
		break;
	case MenuState::INGAME:
		return this->inGameMenuText;
		break;
	case MenuState::FINISH:
		return this->finishMenuText;
		break;
	case MenuState::HOST:
		return this->hostMenuText;
		break;
	case MenuState::JOIN:
		return this->joinMenuText;
		break;
	case MenuState::LOBBY:
		return this->lobbyMenuText;
		break;
	}
}

/*Ustawianie pozycji tekstu.*/
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
		this->multiMenuText[i].setPosition(x, y);
		break;
	case MenuState::INGAME:
		this->inGameMenuText[i].setPosition(x, y);
		break;
	case MenuState::FINISH:
		this->finishMenuText[i].setPosition(x, y);
		break;
	case MenuState::HOST:
		this->hostMenuText[i].setPosition(x, y);
		break;
	case MenuState::JOIN:
		this->joinMenuText[i].setPosition(x, y);
		break;
	case MenuState::LOBBY:
		this->lobbyMenuText[i].setPosition(x, y);
		break;
	}
}

/*Ustawianie koloru tekstu.*/
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
		this->multiMenuText[i].setFillColor(color);
		break;
	case MenuState::INGAME:
		this->inGameMenuText[i].setFillColor(color);
		break;
	case MenuState::FINISH:
		this->finishMenuText[i].setFillColor(color);
		break;
	case MenuState::HOST:
		this->hostMenuText[i].setFillColor(color);
		break;
	case MenuState::JOIN:
		this->joinMenuText[i].setFillColor(color);
		break;
	case MenuState::LOBBY:
		this->lobbyMenuText[i].setFillColor(color);
		break;
	}
}