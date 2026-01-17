#pragma once

#include "GameState.h"

#include <unordered_map>

constexpr std::string_view PROJECT_NAME = "Kobushi Solver";

class MainMenuState : public GameState
{
public:
	MainMenuState(StateManager* state_manager) : GameState(state_manager) {}
	~MainMenuState() override = default;

	void Init() override;
	void HandleInput(const sf::Event event, const sf::RenderWindow& window) override;
	void Update(const sf::Time& delta) override {};
	void Render(sf::RenderTarget& target) override;

private:
	// ELEMENTS
	std::unordered_map<std::string, sf::Text> elements_;

	void LayoutElements();
};