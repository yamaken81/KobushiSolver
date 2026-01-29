#pragma once

#include "Entity.h"
#include "EnemyAI.h"

const float MOVE_DELAY = .2f; // Delay between enemy moves

class Enemy : public Entity
{
public:
	Enemy(LevelMap* level, const sf::Vector2i& gridpos, const EntityType& type);

	// ACCESS
	int GetID() const { return id_; }

	// ENTITY
	void Update(const sf::Time& delta) override;

	// HELPER
	void Build() override;

	static int enemy_count_;

private:
	std::unique_ptr<EnemyAI> ai_;
	int id_;
	sf::Clock moveclock_;
};