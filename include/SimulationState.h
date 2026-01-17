#pragma once

#include "GameState.h"
#include "LevelMap.h"
#include "Tile.h"

#include <SFML/Graphics.hpp>

#include <unordered_map>

class SimulationState : public GameState
{
public:
	SimulationState(StateManager* state_manager);

	void Init() override;
	void HandleInput(const sf::Event event, const sf::RenderWindow& window) override;
	void Update(const sf::Time& delta) override;
	void Render(sf::RenderTarget& target) override;

private:
	bool is_keypress_;
	int win_state_;

	// ELEMENTS
	std::unordered_map<std::string, sf::Text> elements_;

	std::unique_ptr<LevelMap> level_;
};