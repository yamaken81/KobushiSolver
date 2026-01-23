#pragma once

#include "GameState.h"

#include <array>
#include <unordered_map>

constexpr std::string_view PROJECT_NAME = "Kobushi Solver";

constexpr std::array<int, 3> LEVELS = { 1, 3, 4 };

class MainMenuState : public GameState
{
public:
	MainMenuState(StateManager* state_manager) : GameState(state_manager), selected_level_(4) {}
	~MainMenuState() override = default;

	void Init() override;
	void HandleInput(const sf::Event event) override;
	void Update(const sf::Time& delta) override {};
	void Render(sf::RenderTarget& target) override;

private:
	// ELEMENTS
	std::unordered_map<std::string, sf::Text> elements_;
	int selected_level_;

	void LayoutElements();
	void ChangeLevel(int level);
};