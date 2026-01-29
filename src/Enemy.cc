#include "Enemy.h"

#include <iostream>

#include "LevelMap.h"

int Enemy::enemy_count_ = 0;

Enemy::Enemy(LevelMap* level, const sf::Vector2i& gridpos, const EntityType& type)
	: Entity(level, gridpos, type), ai_(nullptr), id_(enemy_count_++)
{
	// Set enemy AI
	switch (type) {
	case EntityType::kDevilbot:	ai_ = std::make_unique<DevilbotAI>(level, this); break;
	default:					std::cerr << "ERR: Enemy::Enemy - Unknown enemy type!\n";
	}

	Build();
}

void Enemy::Update(const sf::Time& delta)
{
	sf::Vector2i offs;
	sf::Vector2i new_pos = GetGridPosition();

	if (level_->IsEnemyTurn() && moveclock_.getElapsedTime().asSeconds() >= MOVE_DELAY) {
		assert(ai_);
		ai_->TakeTurn();
		moveclock_.restart(); // Restart clock for other enemies
	}
}

void Enemy::Build()
{
	sf::RectangleShape shape = GetShape();

	switch (GetType()) {
	case EntityType::kDevilbot:	shape.setFillColor(sf::Color::Red); break;
	}

	SetShape(shape);
}