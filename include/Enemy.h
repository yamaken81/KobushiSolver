#pragma once

#include "Entity.h"

const float MOVE_DELAY = .2f; // Delay between enemy moves

class Enemy : public Entity
{
public:
	Enemy(LevelMap* level, const sf::Vector2i& gridpos, const EntityType& type);

	// ENTITY
	void HandleInput(const sf::Event event, const sf::RenderWindow& window) override {};
	void Update(const sf::Time& delta) override;

	// HELPER
	sf::Vector2i FindPath(const sf::Vector2i& enemy_pos);
	sf::Vector2i ResolveCollisions(const sf::Vector2i& offs);
	void Build() override;

private:
	sf::Clock moveclock_;
	int moves_ = 0; // Number of moves made by the enemy
};