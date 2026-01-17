#pragma once

#include <SFML/Graphics.hpp>

class StateManager;

class GameState
{
protected:
	StateManager* state_manager_ = nullptr;
public:
	GameState(StateManager* state_manager) { state_manager_ = state_manager; }
	virtual ~GameState() = default;

	virtual void Init() = 0;
	virtual void HandleInput(const sf::Event event, const sf::RenderWindow& window) = 0;
	virtual void Update(const sf::Time& delta) = 0;
	virtual void Render(sf::RenderTarget& target) = 0;
};