#include "MainMenuState.h"
#include "StateManager.h"

#include <iostream>

void MainMenuState::Init()
{
	// FONT
	sf::Font& font = state_manager_->GetFont();

	// ELEMENTS
	elements_.emplace("title", sf::Text(font));
	elements_.emplace("level", sf::Text(font));
	elements_.emplace("start", sf::Text(font));
	elements_.emplace("next_lv", sf::Text(font));
	elements_.emplace("prev_lv", sf::Text(font));

	for (auto& [key, text] : elements_) {
		text.setStyle(sf::Text::Regular);
		text.setFillColor(sf::Color::White);
	}

	// TITLE TEXT
	elements_.at("title").setCharacterSize(100);
	elements_.at("title").setStyle(sf::Text::Bold);
	elements_.at("title").setString(std::string(PROJECT_NAME));

	// LEVEL SELECTOR
	elements_.at("level").setCharacterSize(30);
	elements_.at("level").setString("Level: " + std::to_string(DEFAULT_LEVEL));

	elements_.at("next_lv").setCharacterSize(30);
	elements_.at("next_lv").setString("Next");

	elements_.at("prev_lv").setCharacterSize(30);
	elements_.at("prev_lv").setString("Prev");

	// START BUTTON
	elements_.at("start").setCharacterSize(50);
	elements_.at("start").setString("PLAY");

	LayoutElements();
}

void MainMenuState::HandleInput(const sf::Event event)
{
	auto& window = state_manager_->GetWindowContext().GetWindow();
	sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

	// MOUSEOVER
	const std::array<std::string, 3> buttons = { "start", "next_lv", "prev_lv" };
	for (auto& [key, element] : elements_) {
		if (std::find(buttons.begin(), buttons.end(), key) != buttons.end())
			if (element.getGlobalBounds().contains(mouse_pos))
				element.setStyle(sf::Text::Bold);
			else
				element.setStyle(sf::Text::Regular);
	}
	
	// MOUSE BUTTON
	if (const auto* btn_pressed = event.getIf<sf::Event::MouseButtonPressed>()) {
		bool left_clicked = btn_pressed->button == sf::Mouse::Button::Left;
		bool start_hovered = elements_.at("start").getGlobalBounds().contains(mouse_pos);
		bool next_hovered = elements_.at("next_lv").getGlobalBounds().contains(mouse_pos);
		bool prev_hovered = elements_.at("prev_lv").getGlobalBounds().contains(mouse_pos);

		if (left_clicked) {
			if (next_hovered)
				ChangeLevel(1);
			else if (prev_hovered)
				ChangeLevel(-1);
			else if (start_hovered) {
				state_manager_->SetCurrentLevel(selected_level_);
				state_manager_->ChangeState(StateID::kSimulation);
			}
		}
	}

	// KEYBOARD
	if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
		switch (key->code) {
		case sf::Keyboard::Key::Right:	ChangeLevel(1);		break;
		case sf::Keyboard::Key::Left:	ChangeLevel(-1);	break;
		case sf::Keyboard::Key::Enter:
		case sf::Keyboard::Key::Space:
			state_manager_->SetCurrentLevel(selected_level_);
			state_manager_->ChangeState(StateID::kSimulation);
			break;
		}
	}
}

void MainMenuState::Render(sf::RenderTarget& target)
{
	for (const auto& [key, element] : elements_)
		target.draw(element);
}

void MainMenuState::LayoutElements()
{
	constexpr sf::Vector2f margin = { 50.f, 50.f };
	constexpr float padding = 50.f;
	sf::Vector2f cursor = margin;

	elements_.at("title").setPosition(cursor);
	cursor.y += elements_.at("title").getLocalBounds().size.y + (padding * 2);

	elements_.at("level").setPosition(cursor);
	cursor.y += elements_.at("level").getLocalBounds().size.y + (padding / 2);

	elements_.at("next_lv").setPosition(cursor);
	cursor.y += elements_.at("next_lv").getLocalBounds().size.y + (padding / 2);

	elements_.at("prev_lv").setPosition(cursor);
	cursor.y += elements_.at("prev_lv").getLocalBounds().size.y + (padding * 2);

	elements_.at("start").setPosition(cursor);
	cursor.y += elements_.at("start").getLocalBounds().size.y;
}

void MainMenuState::ChangeLevel(int offset)
{
	// Update level
	if (offset > 0) {
		selected_level_ = (selected_level_ % 4) + 1;						// Cycle forwards (i.e. 4 -> 1)
		if (selected_level_ == 2) selected_level_ = 3;						// COMPROMISE: Skip level 2
	} else {
		selected_level_ = (selected_level_ == 1) ? 4 : selected_level_ - 1;	// Cycle backwards (i.e. 1 -> 4)
		if (selected_level_ == 2) selected_level_ = 1;						// COMPROMISE: Skip level 2
	}

	// Update UI
	elements_.at("level").setString("Level: " + std::to_string(selected_level_));
}