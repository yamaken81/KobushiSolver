#pragma once

#include <SFML/Graphics.hpp>

#include "StateManager.h"
#include "WindowContext.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	std::unique_ptr<StateManager> state_manager_;
	WindowContext window_;

	sf::Clock clock_;

	void InitStates();
};