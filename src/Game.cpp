#include "Game.hpp"

Game::Game() : window(sf::VideoMode(1024, 768), "Sokoban", sf::Style::Close)
{
	this->state = GameState::START;
	loadContent();
	Data::getInstance().loadContent();
	menu = new Menu(font);
	menu->loadContent();
	page = 0;

	gameView.setSize(window.getSize().x, window.getSize().y);
	menuView.setSize(window.getSize().x, window.getSize().y);
	menuView.setCenter(window.getSize().x / 2, window.getSize().y / 2);
	this->state = GameState::MENU;
}

Game &Game::getInstance()
{
	static Game instance;
	return instance;
}

Game::~Game()
{
	if (menu)
	{
		delete menu;
	}
	if (level)
	{
		delete level;
	}
	if (!players.empty())
	{
		players.clear();
	}
	delete player;
	if (!targets.empty())
	{
		targets.clear();
	}
	delete target;
	if (!crates.empty())
	{
		crates.clear();
	}
	delete crate;
	if (!texturesCollector.empty())
	{
		texturesCollector.clear();
	}
	if (!sprites.empty())
	{
		sprites.clear();
	}
	if (thread)
	{
		thread->terminate();
		delete thread;
	}
	if (socket)
	{
		socket->disconnect();
		delete socket;
	}
}

/*Wczytywanie zasobow.*/
bool Game::loadContent()
{
	this->textureSize = 32;
	loadTexture("WallTexture", "content/wall.png");
	loadTexture("FloorTexture", "content/floor.png");
	loadTexture("TargetTexture", "content/target.png");
	loadTexture("CrateTexture", "content/crate.png");
	loadTexture("PlayerTextureGreen", "content/player-green.png");
	loadTexture("PlayerTextureRed", "content/player-red.png");
	loadTexture("PlayerTextureBlue", "content/player-blue.png");

	font.loadFromFile("content/Anton-Regular.ttf");

	return true;
}

/*Tworzenie poziomu.*/
void Game::buildLevel(const std::string &name)
{
	//W przypadku gdy level, players, targets i crates istnieja, nalezy je wyczyscic
	if (level)
	{
		delete level;
	}
	if (!players.empty())
	{
		players.clear();
	}
	if (!targets.empty())
	{
		targets.clear();
	}
	if (!crates.empty())
	{
		crates.clear();
	}

	level = new Level();
	level->loadContent(name);

	sprites.clear();
	sprites.resize(level->getSizeY());
	for (auto y = 0; y < level->getSizeY(); y++)
	{
		sprites[y].resize(level->getSizeX());
	}

	//Wypelnianie sprites odpowiednimi teksturami
	for (auto y = 0; y < level->getSizeY(); y++)
	{
		for (auto x = 0; x < level->getSizeX(); x++)
		{
			sprites[y][x].setPosition(x * textureSize, y * textureSize);
			switch (level->getMapValue(x, y))
			{
			case 0:
				break;
			case 1:
				sprites[y][x].setTexture(*getTexture("WallTexture"));
				break;
			case 2:
				sprites[y][x].setTexture(*getTexture("FloorTexture"));
				break;
			}
		}
	}

	for (auto p : level->getPlayersPositions())
	{
		if (level->getPlayersPositions().size() == 1)
		{
			player = new Player(p.first, p.second, textureSize, getTexture("PlayerTextureGreen"));
		}
		else
		{
			switch (players.size())
			{
			case 0:
				player = new Player(p.first, p.second, textureSize, getTexture("PlayerTextureRed"));
				break;
			case 1:
				player = new Player(p.first, p.second, textureSize, getTexture("PlayerTextureBlue"));
				break;
			default:
				player = new Player(p.first, p.second, textureSize, getTexture("PlayerTextureGreen"));
				break;
			}
		}
		players.push_back(player);
	}

	for (auto t : level->getTargetsPositions())
	{
		target = new Target(t.first, t.second, textureSize, getTexture("TargetTexture"));
		targets.push_back(target);
	}

	for (auto c : level->getCratesPositions())
	{
		crate = new Crate(c.first, c.second, textureSize, getTexture("CrateTexture"));
		crates.push_back(crate);
	}

	currentLevelName = name;
	player = players[0];
	gameView.setCenter(level->getSizeX() * textureSize / 2, level->getSizeY() * textureSize / 2);
	window.setView(gameView);
}

/*Glowna petla aplikacji.*/
void Game::run()
{
	while (state != GameState::CLOSE)
	{
		std::cout << "State " << state << std::endl;
		window.clear();
		switch (state)
		{
		case GameState::START:
			showMenu();
			break;
		case GameState::MENU:
			window.setView(menuView);
			showMenu();
			break;
		case GameState::GAME:
			window.setView(gameView);
			showGame();
			break;
		case GameState::CLOSE:
			break;
		}
	}
}

/*Wyswietlanie odpowiedniego menu.*/
void Game::showMenu()
{
	while (state == GameState::MENU)
	{
		std::cout << "Menu state: " << menu->getMenuState() << std::endl;
		switch (menu->getMenuState())
		{
		case Menu::MenuState::MAIN:
			menuMain();
			break;
		case Menu::MenuState::SINGLE:
			menuSingle();
			break;
		case Menu::MenuState::MULTI:
			menuMulti();
			break;
		case Menu::MenuState::INGAME:
			menuInGame();
			break;
		case Menu::MenuState::FINISH:
			menuFinish();
			break;
		case Menu::MenuState::HOST:
			menuHost();
			break;
		case Menu::MenuState::JOIN:
			menuJoin();
			break;
		case Menu::MenuState::LOBBY:
			menuLobby();
			break;
		}
	}
}

/**/
void Game::closeGame()
{
	std::cout << "closing" << std::endl;
	state = GameState::CLOSE;
	getWindow().close();
}

//------------------------Rysowanie i obsluga Menu------------------------//

/*Rysowanie i obsluga glownego menu gry (MAIN).*/
void Game::menuMain()
{
	while (getWindow().isOpen() &&
		   state == GameState::MENU &&
		   menu->getMenuState() == Menu::MenuState::MAIN)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;
		while (getWindow().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				closeGame();
			}
			if (event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
			{
				closeGame();
			}
			else if (menu->getMenuText()[1].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				menu->setMenuState(Menu::MenuState::SINGLE);
			}
			else if (menu->getMenuText()[2].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				menu->setMenuState(Menu::MenuState::MULTI);
			}
			else if (menu->getMenuText()[3].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				closeGame();
			}
		}

		getWindow().clear();
		int positionX, positionY = 0;
		for (auto i = 0u; i < menu->getMenuText().size(); i++)
		{
			positionX = window.getSize().x / 2 - menu->getMenuText()[i].getLocalBounds().width / 2;
			if (i == 1)
				positionY += 70;
			else
				positionY += 50;

			menu->setMenuTextPosition(
				i,
				positionX,
				positionY);
			if (!menu->getMenuText()[i].getGlobalBounds().contains(mouse) || i == 0)
			{
				menu->setMenuTextFillColor(i, sf::Color::Red);
			}
			else if (menu->getMenuText()[i].getGlobalBounds().contains(mouse) && i != 0)
			{
				menu->setMenuTextFillColor(i, sf::Color::Green);
			}
			window.draw(menu->getMenuText()[i]);
		}
		getWindow().display();
	}
}

/*Rysowanie i obsluga menu wyboru poziomu (START).*/
void Game::menuSingle()
{
	page = 0;
	while (getWindow().isOpen() &&
		   state == GameState::MENU &&
		   menu->getMenuState() == Menu::MenuState::SINGLE)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;
		while (getWindow().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				closeGame();
			}
			if (event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
			{
				menu->setMenuState(Menu::MenuState::MAIN);
			}
			else if (menu->getMenuText()[1].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				if (page > 0)
				{
					page--;
				}
			}
			else if (menu->getMenuText()[2].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				if (page < (Data::getInstance().getLevels().size() - 1) / 5)
				{
					page++;
				}
			}
			else if (menu->getMenuText()[menu->getMenuText().size() - 1].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				menu->setMenuState(Menu::MenuState::MAIN);
			}
			else
			{
				for (auto i = 0u; i < 5; i++)
				{
					unsigned x = i + 3 + (page * 5);
					if (x < menu->getMenuText().size())
					{
						if (menu->getMenuText()[x].getGlobalBounds().contains(mouse) &&
							event.type == sf::Event::MouseButtonReleased &&
							event.mouseButton.button == sf::Mouse::Left)
						{
							buildLevel(menu->getMenuText()[x].getString());
							state = GameState::GAME;
						}
					}
				}
			}
		}

		getWindow().clear();
		int positionX = 0, positionY = 0, previousSizeY = 0;
		for (auto i = 0u; i < menu->getMenuText().size(); i++)
		{
			if (i == 0 || ((i >= (page * 5) + 3) && (i < ((page + 1) * 5) + 3)) || i == menu->getMenuText().size() - 1)
			{
				positionX = window.getSize().x / 2 - menu->getMenuText()[i].getLocalBounds().width / 2;
				positionY += previousSizeY + 30;
				//poprawic
				previousSizeY = menu->getMenuText()[i].getLocalBounds().height;
			}
			else
			{
				if (i == 1) //Przycisk previous 5
				{
					positionX = window.getSize().x / 2 - menu->getMenuText()[i].getLocalBounds().width - 50;
					positionY += previousSizeY + 30;
				}
				else if (i == 2) //Przycisk next 5
				{
					positionX = window.getSize().x / 2 + 50;
					previousSizeY = menu->getMenuText()[i].getLocalBounds().height;
				}
			}
			if (i == 0 ||
				(i == 1 && page > 0) ||
				(i == 2 && page < ((menu->getMenuText().size() - 4) / 5)) ||
				((i >= (page * 5) + 3) && (i < ((page + 1) * 5) + 3)) ||
				i == menu->getMenuText().size() - 1)
			{
				menu->setMenuTextPosition(
					i,
					positionX,
					positionY);

				if (!menu->getMenuText()[i].getGlobalBounds().contains(mouse) || i == 0)
				{
					menu->setMenuTextFillColor(i, sf::Color::Red);
				}
				else if (menu->getMenuText()[i].getGlobalBounds().contains(mouse) && i != 0)
				{
					menu->setMenuTextFillColor(i, sf::Color::Green);
				}
				window.draw(menu->getMenuText()[i]);
			}
		}
		getWindow().display();
	}
}

/**/
void Game::menuMulti()
{
	//std::vector<sf::String> multiText = {"Host game", "Join game", "Back"};
	while (getWindow().isOpen() &&
		   state == GameState::MENU &&
		   menu->getMenuState() == Menu::MenuState::MULTI)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;
		while (getWindow().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				closeGame();
			}

			if (event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
			{
				menu->setMenuState(Menu::MenuState::MAIN);
			}
			else if (menu->getMenuText()[1].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				menu->setMenuState(Menu::MenuState::HOST);
			}
			else if (menu->getMenuText()[2].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				menu->setMenuState(Menu::MenuState::JOIN);
			}
			else if (menu->getMenuText()[3].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				menu->setMenuState(Menu::MenuState::MAIN);
			}
		}

		getWindow().clear();
		int positionX, positionY = 0;
		for (auto i = 0u; i < menu->getMenuText().size(); i++)
		{
			positionX = window.getSize().x / 2 - menu->getMenuText()[i].getLocalBounds().width / 2;
			if (i == 1)
				positionY += 70;
			else
				positionY += 50;

			menu->setMenuTextPosition(
				i,
				positionX,
				positionY);
			if (!menu->getMenuText()[i].getGlobalBounds().contains(mouse) || i == 0)
			{
				menu->setMenuTextFillColor(i, sf::Color::Red);
			}
			else if (menu->getMenuText()[i].getGlobalBounds().contains(mouse) && i != 0)
			{
				menu->setMenuTextFillColor(i, sf::Color::Green);
			}
			window.draw(menu->getMenuText()[i]);
		}
		getWindow().display();
	}
}

/**/
void Game::menuInGame()
{
	while (getWindow().isOpen() &&
		   state == GameState::MENU &&
		   menu->getMenuState() == Menu::MenuState::INGAME)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;
		while (getWindow().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				closeGame();
			}

			if (event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
			{
				state = GameState::GAME;
			}
			else if (menu->getMenuText()[0].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				state = GameState::GAME;
			}
			else if (menu->getMenuText()[1].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				buildLevel(currentLevelName);
				state = GameState::GAME;
			}
			else if (menu->getMenuText()[2].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				menu->setMenuState(Menu::MenuState::MAIN);
			}
		}

		getWindow().clear();
		int positionX, positionY = 100, previousSizeY = 0;
		for (auto i = 0u; i < menu->getMenuText().size(); i++)
		{
			positionX = window.getSize().x / 2 - menu->getMenuText()[i].getLocalBounds().width / 2;
			positionY += previousSizeY + 30;
			//poprawic
			previousSizeY = menu->getMenuText()[i].getLocalBounds().height;

			menu->setMenuTextPosition(
				i,
				positionX,
				positionY);
			if (!menu->getMenuText()[i].getGlobalBounds().contains(mouse))
			{
				menu->setMenuTextFillColor(i, sf::Color::Red);
			}
			else if (menu->getMenuText()[i].getGlobalBounds().contains(mouse))
			{
				menu->setMenuTextFillColor(i, sf::Color::Green);
			}
			window.draw(menu->getMenuText()[i]);
		}
		getWindow().display();
	}
}

/**/
void Game::menuFinish()
{
	while (getWindow().isOpen() &&
		   state == GameState::MENU &&
		   menu->getMenuState() == Menu::MenuState::FINISH)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;
		while (getWindow().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				closeGame();
			}
			if (event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
			{
				menu->setMenuState(Menu::MenuState::MAIN);
			}
			else if (menu->getMenuText()[1].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				if (Data::getInstance().nextLevel(currentLevelName))
				{
					buildLevel(Data::getInstance().getNextLevel(currentLevelName));
					state = GameState::GAME;
				}
			}
			else if (menu->getMenuText()[2].getGlobalBounds().contains(mouse) &&
					 event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				state = GameState::MENU;
				menu->setMenuState(Menu::MenuState::MAIN);
			}
		}

		getWindow().clear();
		int positionX, positionY = 0, previousSizeY = 0;
		for (auto i = 0u; i < menu->getMenuText().size(); i++)
		{
			if (i != 1 || (i == 1 && Data::getInstance().nextLevel(currentLevelName)))
			{
				positionX = window.getSize().x / 2 - menu->getMenuText()[i].getLocalBounds().width / 2;
				positionY += previousSizeY + 30;
				//poprawic
				previousSizeY = menu->getMenuText()[i].getLocalBounds().height;

				menu->setMenuTextPosition(
					i,
					positionX,
					positionY);
				if (!menu->getMenuText()[i].getGlobalBounds().contains(mouse) || i == 0)
				{
					menu->setMenuTextFillColor(i, sf::Color::Red);
				}
				else if (menu->getMenuText()[i].getGlobalBounds().contains(mouse) && i != 0)
				{
					menu->setMenuTextFillColor(i, sf::Color::Green);
				}
				window.draw(menu->getMenuText()[i]);
			}
		}
		getWindow().display();
	}
}

/**/
void Game::menuHost()
{
	connected = false;
	if (thread)
	{
		thread->terminate();
		delete thread;
	}
	thread = new sf::Thread([=] { waitForClient(); });
	thread->launch();

	while (getWindow().isOpen() &&
		   state == GameState::MENU &&
		   menu->getMenuState() == Menu::MenuState::HOST &&
		   !connected)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;
		while (getWindow().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				closeGame();
			}
			if (event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
			{
				if (thread)
				{
					thread->terminate();
					delete thread;
				}
				menu->setMenuState(Menu::MenuState::MAIN);
			}
		}

		getWindow().clear();
		int positionX, positionY = 0;
		for (auto i = 0u; i < menu->getMenuText().size(); i++)
		{
			positionX = window.getSize().x / 2 - menu->getMenuText()[i].getLocalBounds().width / 2;
			if (i == 1)
				positionY += 70;
			else
				positionY += 50;

			menu->setMenuTextPosition(
				i,
				positionX,
				positionY);
			if (!menu->getMenuText()[i].getGlobalBounds().contains(mouse) || i == 0 || i == 1)
			{
				menu->setMenuTextFillColor(i, sf::Color::Red);
			}
			else if (menu->getMenuText()[i].getGlobalBounds().contains(mouse) && i != 0 && i != 1)
			{
				menu->setMenuTextFillColor(i, sf::Color::Green);
			}
			window.draw(menu->getMenuText()[i]);
		}
		getWindow().display();
	}

	if (connected)
	{
		if (thread)
		{
			thread->terminate();
			delete thread;
		}
		//thread -> recieveMessage
		menu->setMenuState(Menu::MenuState::LOBBY);
	}
}

/**/
void Game::menuJoin()
{
	connected = false;
	bool result = false;
	inputString = "";
	while (getWindow().isOpen() &&
		   state == GameState::MENU &&
		   menu->getMenuState() == Menu::MenuState::JOIN &&
		   !result)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;
		while (getWindow().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				closeGame();
			}
			if (event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
			{
				menu->setMenuState(Menu::MenuState::MAIN);
			}
			else if (((event.type == sf::Event::KeyPressed &&
					   event.key.code == sf::Keyboard::Return) ||
					  (menu->getMenuText()[3].getGlobalBounds().contains(mouse) &&
					   event.type == sf::Event::MouseButtonReleased &&
					   event.mouseButton.button == sf::Mouse::Left)) &&
					 !inputString.empty())
			{
				result = connectToHost(inputString);
				inputString.clear();
			}
			else if (event.type == sf::Event::KeyPressed &&
					 event.key.code == sf::Keyboard::BackSpace &&
					 !inputString.empty())
			{
				inputString.erase(inputString.size() - 1, 1);
			}
			else if (event.type == sf::Event::TextEntered)
			{
				if ((event.text.unicode >= 48 && event.text.unicode <= 57) || event.text.unicode == 46)
				{
					inputString += static_cast<char>(event.text.unicode);
				}
			}
		}

		getWindow().clear();
		int positionX, positionY = 0;
		for (auto i = 0u; i < menu->getMenuText().size(); i++)
		{
			if (i == 2)
			{
				menu->setMenuTextString(i, inputString);
			}
			positionX = window.getSize().x / 2 - menu->getMenuText()[i].getLocalBounds().width / 2;
			if (i == 1)
				positionY += 70;
			else
				positionY += 50;

			menu->setMenuTextPosition(
				i,
				positionX,
				positionY);
			if (!menu->getMenuText()[i].getGlobalBounds().contains(mouse) || (i != 3 && i != 4))
			{
				menu->setMenuTextFillColor(i, sf::Color::Red);
			}
			else if (menu->getMenuText()[i].getGlobalBounds().contains(mouse) && (i == 3 || i == 4))
			{
				menu->setMenuTextFillColor(i, sf::Color::Green);
			}
			window.draw(menu->getMenuText()[i]);
		}
		getWindow().display();
	}
}

/**/
void Game::menuLobby()
{
	while (getWindow().isOpen() &&
		   state == GameState::MENU &&
		   menu->getMenuState() == Menu::MenuState::LOBBY)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;
		while (getWindow().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				closeGame();
			}
			if (event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
			{
				menu->setMenuState(Menu::MenuState::MAIN);
			}
		}

		getWindow().clear();
		int positionX, positionY = 0;
		for (auto i = 0u; i < menu->getMenuText().size(); i++)
		{
			positionX = window.getSize().x / 2 - menu->getMenuText()[i].getLocalBounds().width / 2;
			if (i == 1)
				positionY += 70;
			else
				positionY += 50;

			menu->setMenuTextPosition(
				i,
				positionX,
				positionY);
			if (!menu->getMenuText()[i].getGlobalBounds().contains(mouse) || i == 0 || i == 1)
			{
				menu->setMenuTextFillColor(i, sf::Color::Red);
			}
			else if (menu->getMenuText()[i].getGlobalBounds().contains(mouse) && i != 0 && i != 1)
			{
				menu->setMenuTextFillColor(i, sf::Color::Green);
			}
			window.draw(menu->getMenuText()[i]);
		}
		getWindow().display();
	}
}

//------------------------Laczenie multiplayer------------------------//

/*Funkcja czeka az klient nawiaze polaczenie.*/
void Game::waitForClient()
{
	socket = new sf::TcpSocket();
	sf::TcpListener listener;
	listener.listen(PORT);
	listener.accept(*socket);
	std::cout << "Connection from: " << socket->getRemoteAddress() << std::endl;
	connected = true;
}

/*Laczenie z hostem.*/
bool Game::connectToHost(const std::string &address)
{
	sf::IpAddress IPAddress(address);
	socket = new sf::TcpSocket();
	auto result = socket->connect(IPAddress, PORT, sf::Time(sf::seconds(15)));
	if (result == sf::Socket::Done)
	{
		std::cout << "Connected to: " << socket->getRemoteAddress() << std::endl;
		connected = true;
		//puszczanie thread z recieveMessage
		menu->setMenuState(Menu::MenuState::LOBBY);
		return true;
	}
	else
	{
		return false;
	}
}

//------------------------Rysowanie i obsluga Gry------------------------//

/*Obsluga gry.*/
void Game::showGame()
{
	while (getWindow().isOpen() && state == GameState::GAME)
	{
		sf::Event event;
		while (getWindow().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				closeGame();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					state = GameState::MENU;
					menu->setMenuState(Menu::MenuState::INGAME);
				}
				if (event.key.code == sf::Keyboard::Up)
				{
					if (!checkColission(0, -1, true))
						move(0, -1);
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					if (!checkColission(0, 1, true))
						move(0, 1);
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					if (!checkColission(1, 0, true))
						move(1, 0);
				}
				else if (event.key.code == sf::Keyboard::Left)
				{
					if (!checkColission(-1, 0, true))
						move(-1, 0);
				}
			}
		}

		getWindow().clear();
		drawGame();
		if (checkFinish())
		{
			state = GameState::MENU;
			menu->setMenuState(Menu::MenuState::FINISH);
		}
		getWindow().display();
	}
}

/*Rysowanie gry.*/
void Game::drawGame()
{
	for (auto y = 0; y < level->getSizeY(); y++)
	{
		for (auto x = 0; x < level->getSizeX(); x++)
		{
			getWindow().draw(sprites[y][x]);
		}
	}

	for (auto t : targets)
	{
		getWindow().draw(t->getSprite());
	}

	for (auto c : crates)
	{
		getWindow().draw(c->getSprite());
	}

	for (auto p : players)
	{
		getWindow().draw(p->getSprite());
	}
	window.setView(gameView);
}

//------------------------Dodatkowe funkcje------------------------//

/**/
void Game::move(int moveX, int moveY)
{
	player->move(moveX, moveY);
}

/*Test na kolizje - zwraca true jezeli nie da sie wejsc na pole, w przeciwnym wypadku false.
Na dane pole mozna wejsc, jezeli jest puste, badz jezeli obiekt zajmujacy dane pole mozna
przesunac na nastepne pole.*/
bool Game::checkColission(int moveX, int moveY, bool playerFlag)
{
	int posX = player->getPositionX(), posY = player->getPositionY();
	//std::cout << "Pozycja x: " << posX << ", y: " << posY << std::endl;
	//std::cout << "Move x: " << posX + moveX << ", y: " << posY + moveY << std::endl;
	//std::cout << "Pole: " << level->getMapValue(posX + moveX, posY + moveY) << std::endl;

	//test czy mozna wejsc na pole
	if (level->getMapValue(posX + moveX, posY + moveY) != 0 && level->getMapValue(posX + moveX, posY + moveY) != 1)
	{
		//dla kazdego obiektu typu Crate
		for (auto c : crates)
		{
			//test czy obiekt Crate nie zajmuje pola
			if (c->getPositionX() == posX + moveX && c->getPositionY() == posY + moveY)
			{
				//test czy sprawdzamy dla gracza
				if (playerFlag)
				{
					//test czy mozna przesunac obiekt Crate
					if (!checkColission(2 * moveX, 2 * moveY, false))
					{
						//przesuwamy obiekt Crate
						c->move(moveX, moveY);
						return false;
					}
					return true;
				}
				else
				{
					return true;
				}
			}
		}
		return false;
	}
	else
	{
		return true;
	}

	return true;
}

/*Checks whether all targets are covered by crates.*/
bool Game::checkFinish()
{
	unsigned count = 0;
	for (auto t : targets)
	{
		for (auto c : crates)
		{
			if (t->getPositionX() == c->getPositionX() && t->getPositionY() == c->getPositionY())
				count++;
		}
	}

	if (count == this->targets.size())
		return true;
	return false;
}

/**/
bool Game::checkNextLevel()
{
	std::map<std::string, std::string>::iterator it = Data::getInstance().getLevels().find(currentLevelName);
	if (std::next(it) != Data::getInstance().getLevels().end())
	{
		return true;
	}
	return false;
}

/*If texturesCollector doesn't contain texture with textureName then loads it from file using texturePath.*/
void Game::loadTexture(const std::string &textureName, const std::string &texturePath)
{
	auto textureIterator = texturesCollector.find(textureName);
	if (textureIterator == texturesCollector.end())
	{
		sf::Texture *result = new sf::Texture();
		result->loadFromFile(texturePath);
		texturesCollector.insert({textureName, result});
	}
}

/*If texturesCollector contains texture with textureName then returns it.
Otherwise returns nullptr.*/
sf::Texture *Game::getTexture(const std::string &textureName)
{
	auto textureIterator = texturesCollector.find(textureName);
	if (textureIterator == texturesCollector.end())
	{
		return nullptr;
	}
	return textureIterator->second;
}

void Game::sendPacket(int type)
{
	MyPacketData data;
	switch(type)
	{
		case 0:
			data.type = type;
			//data.message
			data.moveX = data.moveY = 0;
			break;
		case 1:
			//
			break;
	}
	sf::Packet packetSend;

}

void Game::recievePacket()
{
	while (connected)
	{
		MyPacketData data;
		sf::Packet packetRecieve;
		if (socket->receive(packetRecieve) == sf::Socket::Disconnected)
		{
			std::cout << "Lost connection." << std::endl;
			socket->disconnect();
			//clear messages
			//change status
			connected = false;
		}
		if (packetRecieve >> data)
		{
			switch (data.type)
			{
			case 0:
				//message
				break;
			case 1:
				//
				//
				break;
			}
		}
	}
}

void Game::addMessage(const std::string &)
{
	//
}

/*
29.04 22:51
Zacmienie mozgu - nie moge sie juz skupic
ostatnie zmiany w recievePacket
obecny cel:
- przesylac odpowiednie paczki miedzy hostem a klientem
w zaleznosci od paczki albo dodawac wiadomosc albo aktualizowac pozycje drugiego gracza
- problemy z polaczeniem

co trzeba zrobic/przemyslec:
- wybor poziomu w lobby
- jak sie bedzie zachowywac gra jezeli bedzie tylko wysylac paczki po wykonaniu ruchu
- co sie dzieje kiedy po polaczeniu jeden z graczy wyjdzie do/otworzy menu
- czat w widoku gry MP
- swoje zycie
*/