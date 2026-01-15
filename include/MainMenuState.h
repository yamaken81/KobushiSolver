#pragma once

#include "GameState.h"

constexpr std::string_view PROJECT_NAME = "Kobushi Solver";

class MainMenuState : public GameState
{
public:
	MainMenuState()
		: font_(), txt_title_(font_), btn_start_(font_) {}
	~MainMenuState() override = default;

	void Init() override;
	void HandleInput(const sf::Event event, const sf::RenderWindow& window) override;
	void Update(const sf::Time& delta) override {};
	void Render(sf::RenderTarget& target) override;

private:
	// ELEMENTS
	sf::Font font_;
	sf::Text txt_title_;
	sf::Text btn_start_;
};