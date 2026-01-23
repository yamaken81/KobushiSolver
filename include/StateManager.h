#pragma once

#include "GameState.h"
#include "WindowContext.h"

#include <map>
#include <memory>

enum StateID
{
	kMainMenu,
	kSimulation
};

class StateManager
{
public:
	StateManager(WindowContext& window);
	~StateManager() = default;

	// STATE MANAGER
	void AddState(const StateID id, std::unique_ptr<GameState> state);
	void ChangeState(const StateID id);

	// GAME LOOP
	void HandleInput(const sf::Event event);
	void Update(sf::Time delta);
	void Render(sf::RenderTarget& target);

	inline WindowContext& GetWindowContext() { return window_; }
	inline sf::Font& GetFont() { return font_; }
	inline int GetCurrentLevel() const { return current_level_; }
	inline void SetCurrentLevel(int level) { current_level_ = level; }
private:
	std::map<StateID, std::unique_ptr<GameState>> states_;
	GameState* active_state_;

	WindowContext& window_;
	sf::Font font_;

	int current_level_;
};