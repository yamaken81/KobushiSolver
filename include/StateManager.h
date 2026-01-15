#pragma once

#include "GameState.h"

#include <map>
#include <memory>

enum StateID
{
	kMainMenu,
	kSimulation
};

class StateManager
{
protected:
	static StateManager* instance_;

public:
	static StateManager* GetInstance();				// SINGLETON CREATION
	StateManager(StateManager&) = delete;			// SINGLETON CANNOT BE CLONED
	void operator=(const StateManager&) = delete;	// SINGLETON CANNOT BE ASSIGNABLE

	StateManager()
		: active_state_(nullptr) {
	}
	~StateManager() = default;

	// STATE MANAGER
	void AddState(const StateID id, std::unique_ptr<GameState> state);
	void ChangeState(const StateID id);

	// GAME LOOP
	void HandleInput(const sf::Event event, const sf::RenderWindow& window);
	void Update(sf::Time delta);
	void Render(sf::RenderTarget& target);

private:
	std::map<StateID, std::unique_ptr<GameState>> states_;
	GameState* active_state_;
};