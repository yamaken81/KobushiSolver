#pragma once

#include <unordered_map>

#include "GameState.h"

constexpr std::string_view PROJECT_NAME = "Kobushi Solver";

class MainMenuState : public GameState
{
public:
	MainMenuState()
		: font_() {}
	~MainMenuState() override = default;

	void Init() override;
	void HandleInput(const sf::Event event, const sf::RenderWindow& window) override;
	void Update(const sf::Time& delta) override {};
	void Render(sf::RenderTarget& target) override;

private:
	// ELEMENTS
	sf::Font font_; // TODO: Let the StateManager handle this.
	std::unordered_map<std::string, sf::Text> elements_;

	void LayoutElements();
};