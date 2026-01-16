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
	std::unique_ptr<StateManager> state_manager_;

	sf::RenderWindow window_;
	sf::Clock clock_;

	void InitStates();
};