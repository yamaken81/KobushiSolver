#pragma once

#include <SFML/Graphics.hpp>

#include "StateManager.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	sf::RenderWindow window_;
	sf::Clock clock_;
	StateManager* gsmanager_;

	void InitStates();
};