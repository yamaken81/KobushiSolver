#pragma once

#include "GameState.h"
#include "LevelMap.h"
#include "Tile.h"

#include <SFML/Graphics.hpp>

class SimulationState : public GameState
{
public:
	SimulationState();

	void Init() override;
	void HandleInput(const sf::Event event, const sf::RenderWindow& window) override;
	void Update(const sf::Time& delta) override;
	void Render(sf::RenderTarget& target) override;

private:
	bool is_keypress_;
	int win_state_;

	// ELEMENTS
	sf::Font font_;
	sf::Text txt_win_;
	sf::Text txt_gems_;

	std::unique_ptr<LevelMap> level_;
};