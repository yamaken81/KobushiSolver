#include "Game.h"
#include "MainMenuState.h"
#include "SimulationState.h"

Game::Game()
	: window_(sf::VideoMode({ 1536, 864 }), std::string(PROJECT_NAME)),
	  gsmanager_(StateManager::GetInstance())
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
		sf::Time delta = clock.restart();

		// Process events
		while (const std::optional<sf::Event> event = window_.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window_.close();

			gsmanager_->HandleInput(*event, window_);
		}

		// Update and render
		window_.clear();
		gsmanager_->Update(delta);
		gsmanager_->Render(window_);
		window_.display();
	}
}

void Game::InitStates()
{
	gsmanager_->AddState(StateID::kMainMenu, std::make_unique<MainMenuState>());
	gsmanager_->AddState(StateID::kSimulation, std::make_unique<SimulationState>());

	gsmanager_->ChangeState(StateID::kMainMenu);
}
