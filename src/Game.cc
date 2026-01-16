#include "Game.h"
#include "MainMenuState.h"
#include "SimulationState.h"

Game::Game()
	: window_(sf::VideoMode({ 1536, 864 }), std::string(PROJECT_NAME)),
	  state_manager_(StateManager::GetInstance())
{
	InitStates();
}

Game::~Game()
{
}

void Game::Run()
{
	// Init clock for frame timing
	sf::Clock clock;

	// Main game loop
	while (window_.isOpen())
	{
		// Restart the clock to get the time since the last frame
		// NOTE: Delta may be unnecessary due to turn-based state of the game.
		sf::Time delta = clock.restart();

		// Process events
		while (const std::optional<sf::Event> event = window_.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window_.close();

			state_manager_->HandleInput(*event, window_);
		}

		// Update and render
		window_.clear();
		state_manager_->Update(delta);
		state_manager_->Render(window_);
		window_.display();
	}
}

void Game::InitStates()
{
	state_manager_->AddState(StateID::kMainMenu, std::make_unique<MainMenuState>());
	state_manager_->AddState(StateID::kSimulation, std::make_unique<SimulationState>());

	state_manager_->ChangeState(StateID::kMainMenu);
}
