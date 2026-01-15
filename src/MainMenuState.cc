#include "MainMenuState.h"
#include "StateManager.h"

#include <iostream>

void MainMenuState::Init()
{
	// FONT
	if (!font_.openFromFile("res/OpenSans.ttf"))
	{
		std::cerr << "ERR: Font could not be loaded!\n";
		return;
	}

	// TITLE TEXT
	txt_title_.setCharacterSize(100);
	txt_title_.setStyle(sf::Text::Bold);
	txt_title_.setFillColor(sf::Color::White);
	txt_title_.setString(std::string(PROJECT_NAME));

	// START BUTTON
	btn_start_.setCharacterSize(50);
	btn_start_.setStyle(sf::Text::Regular);
	btn_start_.setFillColor(sf::Color::White);
	btn_start_.setString("PLAY");
}

void MainMenuState::HandleInput(const sf::Event event, const sf::RenderWindow& window)
{
	sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

	// MOUSEOVER
	if (btn_start_.getGlobalBounds().contains(mouse_pos))
		btn_start_.setStyle(sf::Text::Bold);
	else
		btn_start_.setStyle(sf::Text::Regular);
	
	// MOUSE BUTTON
	if (const auto* btn_pressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (btn_pressed->button == sf::Mouse::Button::Left &&
			btn_start_.getGlobalBounds().contains(mouse_pos))
			StateManager::GetInstance()->ChangeState(StateID::kSimulation);
	}
}

void MainMenuState::Render(sf::RenderTarget& target)
{
	// POSITIONING
	float vert_spacing = 100.f;
	sf::Vector2f window_size = sf::Vector2f(target.getSize());
	sf::Vector2f title_size = txt_title_.getLocalBounds().size;
	sf::Vector2f start_size = btn_start_.getLocalBounds().size;

	txt_title_.setOrigin({ title_size.x / 2.f, title_size.y / 2.f });
	btn_start_.setOrigin({ start_size.x / 2.f, start_size.y / 2.f });

	float total_height = title_size.y + vert_spacing + start_size.y;
	float current_y = (window_size.y / 2.f) - (total_height / 2.f);

	txt_title_.setPosition({ (window_size.x / 2.f), current_y + (title_size.y / 2.f) });
	current_y += title_size.y + vert_spacing;
	
	btn_start_.setPosition({ (window_size.x / 2.f), current_y + (start_size.y / 2.f) });

	// DRAW
	target.draw(txt_title_);
	target.draw(btn_start_);
}
