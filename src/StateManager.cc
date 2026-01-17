#include "StateManager.h"

#include <iostream>


StateManager::StateManager(sf::RenderWindow& window)
	: window_(window), active_state_(nullptr)
{
	// Load font
	if (!font_.openFromFile("res/OpenSans.ttf"))
		throw std::runtime_error("ERR: Font could not be loaded\n");
}

void StateManager::AddState(const StateID id, std::unique_ptr<GameState> state)
{
	states_.emplace(id, std::move(state));
}

void StateManager::ChangeState(const StateID id)
{
	// Find the state with the given ID and set it as the active state
	auto it = states_.find(id);
	if (it != states_.end())
	{
		active_state_ = it->second.get();
		active_state_->Init();
	}
	else
		throw std::runtime_error("ERR: State being changed to doesn't exist.");
}

void StateManager::HandleInput(const sf::Event event, const sf::RenderWindow& window)
{
	if (active_state_)
		active_state_->HandleInput(event, window);
}

void StateManager::Update(sf::Time delta)
{
	if (active_state_)
		active_state_->Update(delta);
}

void StateManager::Render(sf::RenderTarget& target)
{
	if (active_state_)
		active_state_->Render(target);
}