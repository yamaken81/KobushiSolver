#include "SimulationState.h"

#include "StateManager.h"

SimulationState::SimulationState()
	: font_(), txt_win_(font_), txt_gems_(font_), is_keypress_(false), win_state_(0)
{
	level_ = std::make_unique<LevelMap>();
}

void SimulationState::Init()
{
	// FONT
	if (!font_.openFromFile("res/OpenSans.ttf"))
	{
		std::cerr << "ERR: Font could not be loaded!\n";
		return;
	}

	// WIN TEXT
	txt_win_.setCharacterSize(20);
	txt_win_.setStyle(sf::Text::Bold);
	txt_win_.setFillColor(sf::Color::White);

	// GEM TEXT
	txt_gems_.setCharacterSize(20);
	txt_gems_.setFillColor(sf::Color::White);
	txt_gems_.setString(std::string("GEMS: 0"));

	// LEVEL
	is_keypress_ = false;
	win_state_ = 0;
	level_->Init();
}

void SimulationState::HandleInput(const sf::Event event, const sf::RenderWindow& window)
{
	if (const auto* key = event.getIf<sf::Event::KeyPressed>())
	{
		if (key->code == sf::Keyboard::Key::Escape)
		{
			StateManager::GetInstance()->ChangeState(kMainMenu); // Go back to main menu
			return;
		}
	}

	if (event.getIf<sf::Event::KeyPressed>() && !is_keypress_ && !level_->IsEnemyTurn() && !win_state_) // to prevent multiple key presses from stacking
	{
		// Handle input for the level
		level_->HandleInput(event, window);

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
	if (player_tile->GetType() == TileType::kStairs)
		win_state_ = 1;	// Player reached the stairs, level is complete
	else if (level_->IsPlayerCaught())
		win_state_ = -1;
	else
		win_state_ = 0; // Player decides not to leave the level for whatever reason

	// Update gem text
	txt_gems_.setString("GEMS: " + std::to_string(player->GetGems()));
}

void SimulationState::Render(sf::RenderTarget& target)
{
	// Calculate grid origin to center the level on the screen
	float grid_origin_x = (target.getSize().x - (TILE_SIZE * ROW_SIZE)) / 2.0f;
	float grid_origin_y = (target.getSize().y - (TILE_SIZE * COL_SIZE)) / 2.0f;

	level_->Render(target, sf::Vector2f({ grid_origin_x, grid_origin_y }));

	float wintxt_x = (target.getSize().x / 2.f) - (txt_win_.getLocalBounds().size.x / 2.f);
	float wintxt_y = 5.f; // Tiny bit at the top of the window with a margin
	txt_win_.setPosition({ wintxt_x, wintxt_y });

	float gemtxt_hmargin = 150.f; // Horizontal spacing for gem text
	float gemtxt_x = (grid_origin_x - gemtxt_hmargin);
	float gemtxt_y = grid_origin_y;
	txt_gems_.setPosition({ gemtxt_x, gemtxt_y });

	switch (win_state_)
	{
	case 1:
		txt_win_.setString(std::string("LEVEL COMPLETE! Press Esc to exit to main menu."));
		break;
	case -1:
		txt_win_.setString(std::string("GAME OVER! Press Esc to exit to main menu."));
		break;
	default:
		txt_win_.setString(std::string(""));
	}

	target.draw(txt_gems_);
	target.draw(txt_win_);
}
