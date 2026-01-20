#include "Enemy.h"

#include "LevelMap.h"

Enemy::Enemy(LevelMap* level, const sf::Vector2i& gridpos, const EntityType& type)
	: Entity(level, gridpos, type), moves_(0)
{
	Build();
}

void Enemy::Update(const sf::Time& delta)
{
	sf::Vector2i offs;
	sf::Vector2i new_pos = GetGridPosition();

	if (GetLevelMap()->IsEnemyTurn() && moveclock_.getElapsedTime().asSeconds() >= MOVE_DELAY)
	{
		switch (GetType())
		{
		case EntityType::kDevilbot:
			offs = FindPath(new_pos);
			new_pos = ResolveCollisions(offs);

			SetGridPosition(new_pos);
			Layout();

			switch (moves_)
			{
			case 0:
				moves_++; // Increment moves after first move
				break;
			case 1:
				moves_ = 0; // Reset moves after two moves
				GetLevelMap()->SetEnemyTurn(false); // End enemy turn 
				break;
			}

			moveclock_.restart();
			break;
		default:
			moveclock_.restart(); // Reset clock for other enemy types
		}
	}
}

sf::Vector2i Enemy::FindPath(const sf::Vector2i& enemy_pos)
{
	LevelMap* level = GetLevelMap();
	sf::Vector2i player_pos = level->GetPlayer()->GetGridPosition();
	sf::Vector2i pos_diff = player_pos - enemy_pos;
	sf::Vector2i offs = { 0, 0 };

	if (pos_diff.x > 0)
		offs.x = 1;
	if (pos_diff.x < 0)
		offs.x = -1;
	if (pos_diff.y > 0)
		offs.y = 1;
	if (pos_diff.y < 0)
		offs.y = -1;

	return offs;
}

sf::Vector2i Enemy::ResolveCollisions(const sf::Vector2i& offs)
{
	LevelMap* level = GetLevelMap();
	const sf::Vector2i enemy_pos = GetGridPosition();
	
	// First move has bias on horizontal movement, second on vertical.
	sf::Vector2i first_move = (moves_ == 0) ? sf::Vector2i(offs.x, 0) : sf::Vector2i(0, offs.y);
	sf::Vector2i second_move = (moves_ == 0) ? sf::Vector2i(0, offs.y) : sf::Vector2i(offs.x, 0);

	// Try first move
	if (first_move.x != 0 || first_move.y != 0) {
		sf::Vector2i dest = enemy_pos + first_move;
		if (!level->DoesCollide(enemy_pos, dest)) return dest;
	}

	// Try second move
	if (second_move.x != 0 || second_move.y != 0) {
		sf::Vector2i dest = enemy_pos + second_move;
		if (!level->DoesCollide(enemy_pos, dest)) return dest;
	}

	return enemy_pos;
}

void Enemy::Build()
{
	sf::RectangleShape shape = GetShape();

	switch (GetType())
	{
	case EntityType::kDevilbot:
		shape.setFillColor(sf::Color::Red);
		break;
	}

	SetShape(shape);
}