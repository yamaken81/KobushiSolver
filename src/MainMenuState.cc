#include "MainMenuState.h"
#include "StateManager.h"

#include <iostream>

void MainMenuState::Init()
{
	// FONT
	sf::Font& font = state_manager_->GetFont();

	// TITLE TEXT
	elements_.emplace("title", sf::Text(font));
	elements_.at("title").setCharacterSize(100);
	elements_.at("title").setStyle(sf::Text::Bold);
	elements_.at("title").setFillColor(sf::Color::White);
	elements_.at("title").setString(std::string(PROJECT_NAME));

	// START BUTTON
	elements_.emplace("start", sf::Text(font));
	elements_.at("start").setCharacterSize(50);
	elements_.at("start").setStyle(sf::Text::Regular);
	elements_.at("start").setFillColor(sf::Color::White);
	elements_.at("start").setString("PLAY");

	LayoutElements();
}

void MainMenuState::HandleInput(const sf::Event event, const sf::RenderWindow& window)
{
	sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

	// MOUSEOVER
	if (elements_.at("start").getGlobalBounds().contains(mouse_pos))
		elements_.at("start").setStyle(sf::Text::Bold);
	else
		elements_.at("start").setStyle(sf::Text::Regular);
	
	// MOUSE BUTTON
	if (const auto* btn_pressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (btn_pressed->button == sf::Mouse::Button::Left &&
			elements_.at("start").getGlobalBounds().contains(mouse_pos))
			state_manager_->ChangeState(StateID::kSimulation);
	}
}

void MainMenuState::Render(sf::RenderTarget& target)
{
	target.draw(elements_.at("title"));
	target.draw(elements_.at("start"));
}

void MainMenuState::LayoutElements()
{
	constexpr sf::Vector2f margin = { 50.f, 50.f };
	constexpr float padding = 50.f;
	sf::Vector2f cursor = margin;

	// Set origins
	elements_.at("title").setOrigin({0.f, 0.f});
	elements_.at("start").setOrigin({ 0.f, 0.f });

	elements_.at("title").setPosition(cursor);
	cursor.y += elements_.at("title").getLocalBounds().size.y + padding;

	elements_.at("start").setPosition(cursor);
	cursor.y += elements_.at("start").getLocalBounds().size.y + padding;
}