#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player(LevelMap* level, const sf::Vector2i& gridpos);

	// ACCESS
	int GetGems() const { return gems_; }

	// ENTITY
	void HandleInput(const sf::Event event, const sf::RenderWindow& window) override;
	void Update(const sf::Time& delta) override;

	// HELPER
	void Build() override;

	// DEBUG
	void DEBUG_LogMovement();
	void DEBUG_LogCollision(const sf::Vector2i& new_pos);

private:
	int gems_ = 0;
};
