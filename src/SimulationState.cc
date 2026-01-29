#include "SimulationState.h"

#include "StateManager.h"

void SimulationState::Init()
{
	// FONT
	sf::Font& font = state_manager_->GetFont();

	// WIN TEXT
	elements_.emplace("win", sf::Text(font));
	elements_.at("win").setCharacterSize(20);
	elements_.at("win").setStyle(sf::Text::Bold);
	elements_.at("win").setFillColor(sf::Color::White);

	// GEM TEXT
	elements_.emplace("gems", sf::Text(font));
	elements_.at("gems").setCharacterSize(20);
	elements_.at("gems").setFillColor(sf::Color::White);
	elements_.at("gems").setString(std::string("GEMS: 0"));

	// LEVEL
	is_keypress_ = false;
	win_state_ = 0;
	level_->Init(state_manager_->GetCurrentLevel());

	LayoutElements();
}

void SimulationState::HandleInput(const sf::Event event)
{
	if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
		if (key->code == sf::Keyboard::Key::Escape) {
			state_manager_->ChangeState(kMainMenu); // Go back to main menu
#ifdef _DEBUG
			std::cout << "INFO: Simulation ended.\n\n";
#endif
			return;
		}
	}

	bool enemy_turn = level_->IsEnemyTurn() && level_->EnemiesExist();
	if (event.getIf<sf::Event::KeyPressed>() && !is_keypress_ && !enemy_turn && !win_state_) {
		level_->HandleInput(event);
		is_keypress_ = true;
	}
	
	// Reset pressing state
	if (event.getIf<sf::Event::KeyReleased>())
		is_keypress_ = false;
}

void SimulationState::Update(const sf::Time& delta)
{
	level_->Update(delta);

	Player* player = level_->GetPlayer();
	Tile* player_tile = level_->GetTileAt(player->GetGridPosition());

	// Update win condition
	if (player_tile->GetType() == TileType::kStairs) {
		win_state_ = 1;
	} else if (level_->IsPlayerCaught()) {
		win_state_ = -1;
	}
	else
		win_state_ = 0; // Player decides not to leave the level for whatever reason

	// Update win text
	switch (win_state_) {
	case 1:
		elements_.at("win").setString(std::string("LEVEL COMPLETE! Press Esc to exit to main menu."));
		LayoutElements();
		break;
	case -1:
		elements_.at("win").setString(std::string("GAME OVER! Press Esc to exit to main menu."));
		LayoutElements();
		break;
	default:
		elements_.at("win").setString(std::string(""));
	}

	// Update gem text
	elements_.at("gems").setString("GEMS: " + std::to_string(player->GetGems()));
}

void SimulationState::Render(sf::RenderTarget& target)
{
	// Calculate grid origin to center the level on the screen
	//float grid_origin_x = (target.getSize().x - (TILE_SIZE * ROW_SIZE)) / 2.0f;
	//float grid_origin_y = (target.getSize().y - (TILE_SIZE * COL_SIZE)) / 2.0f;
	sf::Vector2f window_size = state_manager_->GetWindowContext().GetWindowSize();
	sf::Vector2f tile_offset = sf::Vector2f({ TILE_SIZE / 2.0f, TILE_SIZE / 2.0f });
	sf::Vector2f grid_origin = (window_size - tile_offset) / 2.0f;

	level_->Render(target, grid_origin);

	target.draw(elements_.at("gems"));
	target.draw(elements_.at("win"));
}

void SimulationState::LayoutElements() {
	sf::Vector2f window_size = state_manager_->GetWindowContext().GetWindowSize();

	float wintxt_x = (window_size.x / 2.f) - (elements_.at("win").getLocalBounds().size.x / 2.f);
	float wintxt_y = 5.f; // Tiny bit at the top of the window with a margin
	elements_.at("win").setPosition({ wintxt_x, wintxt_y });

	// Calculate grid origin to center the level on the screen
	sf::Vector2f gridbounds = level_->GetGridbounds();

	constexpr float gemtxt_hmargin = 150.f; // Horizontal spacing for gem text
	float gemtxt_x = (gridbounds.x - gemtxt_hmargin);
	float gemtxt_y = gridbounds.y;
	elements_.at("gems").setPosition({ gemtxt_x, gemtxt_y });
}