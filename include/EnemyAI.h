#pragma once

#include <SFML/Graphics.hpp>

class LevelMap;
class Enemy;

class EnemyAI
{
public:
	EnemyAI(LevelMap* level, Enemy* self) : level_(level), self_(self) {}
	virtual ~EnemyAI() = default;

	sf::Vector2i FindPlayer();
	virtual void TakeTurn();

protected:
	LevelMap* level_;
	Enemy* self_;
};

class DevilbotAI : public EnemyAI
{
public:
	DevilbotAI(LevelMap* level, Enemy* self) : EnemyAI(level, self) {};
	~DevilbotAI() override = default;

	void TakeTurn() override;

private:
	int moves_ = 0;

	sf::Vector2i FindPath();
};