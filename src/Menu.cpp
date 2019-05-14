#include "Menu.hpp"

Menu::Menu(sf::Font &font)
{
	this->font = font;
	state = MenuState::MAIN;
}

Menu::~Menu()
{
	mainMenuText.clear();
	singleMenuText.clear();
	multiMenuText.clear();
	inGameMenuText.clear();
	finishMenuText.clear();
	hostMenuText.clear();
	joinMenuText.clear();
	lobbyMenuText.clear();
}

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
	for (auto l : Data::getInstance().getLevels(Data::GameMode::SINGLEPLAYER))
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

	std::vector<sf::String> joinText = {"Join game", "Type IP address:", "", "Join", "Menu"};
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

	std::vector<sf::String> lobbyText = {"Lobby"};
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
	/*Wypelnianie lobbyMenuText nazwami poziomow.*/
	for (auto l : Data::getInstance().getLevels(Data::GameMode::MULTIPLAYER))
	{
		sf::Text t;
		t.setFont(font);
		t.setCharacterSize(25);
		t.setString(l.first);
		lobbyMenuText.push_back(t);
	}
	/*Dodawanie na sam koniec lobbyMenuText tekstu Back.*/
	{
		sf::Text t;
		t.setFont(font);
		t.setCharacterSize(30);
		t.setString("Menu");
		lobbyMenuText.push_back(t);
	}

	return true;
}

/*Funkcja zwraca odpowiednie menu w zaleznosci od zmiennej state.*/
std::vector<sf::Text> &Menu::getMenuText()
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
	getMenuText()[i].setPosition(x, y);
}

/*Ustawianie koloru tekstu.*/
void Menu::setMenuTextFillColor(int i, sf::Color color)
{
	getMenuText()[i].setFillColor(color);
}

//Ustawianie tresci tekstu
void Menu::setMenuTextString(int i, const std::string &str)
{
	getMenuText()[i].setString(str);
}