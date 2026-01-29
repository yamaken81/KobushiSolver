#include "EnemyAI.h"

#include <iostream>
#include <format>

#include "LevelMap.h"

void DEBUG_LogMovement(LevelMap* level, sf::Vector2i gridpos, std::string enemystr)
{
	const Tile* tile_at_enemy = level->GetTileAt(gridpos);
	std::cout << enemystr <<
		": [" << tile_at_enemy->GetGridPosition().x << "," << tile_at_enemy->GetGridPosition().y <<
		"] Type: " << tile_at_enemy->GetType() <<
		", Borders: " << tile_at_enemy->GetBorders()[0] << tile_at_enemy->GetBorders()[1] <<
		tile_at_enemy->GetBorders()[2] << tile_at_enemy->GetBorders()[3] << "\n";
}

void DEBUG_LogCollision(LevelMap* level, sf::Vector2i gridpos, sf::Vector2i new_pos, std::string enemystr)
{
	if (level->DoesCollide(gridpos, new_pos))
		std::cout << enemystr << ": Collision detected! Movement blocked.\n";
}

void EnemyAI::TakeTurn()
{
	std::cerr << "ERR: EnemyAI::TakeTurn not overriden!\n";
}

sf::Vector2i EnemyAI::FindPlayer()
{
	sf::Vector2i player_pos = level_->GetPlayer()->GetGridPosition();
	sf::Vector2i enemy_pos = self_->GetGridPosition();
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

// DEVILBOT: Takes two moves per turn, with a bias on horizontal movement first.

void DevilbotAI::TakeTurn()
{
	sf::Vector2i new_pos = FindPath();

	self_->SetGridPosition(new_pos);
	self_->Layout();

	switch (moves_) {
	case 0:
		moves_++; // Increment moves after first move
		break;
	case 1:
		moves_ = 0; // Reset moves after two moves
		level_->SetEnemyTurn(false); // End enemy turn 
		break;
	}
}

sf::Vector2i DevilbotAI::FindPath()
{
	const sf::Vector2i enemy_pos = self_->GetGridPosition();
	auto path = EnemyAI::FindPlayer(); // Get path to player

	// First move has bias on horizontal movement, second on vertical.
	sf::Vector2i first_move = (moves_ == 0) ? sf::Vector2i(path.x, 0) : sf::Vector2i(0, path.y);
	sf::Vector2i second_move = (moves_ == 0) ? sf::Vector2i(0, path.y) : sf::Vector2i(path.x, 0);

	std::vector<sf::Vector2i> possible_moves = { first_move, second_move };
	sf::Vector2i dest;
	for (const auto& offs : possible_moves) {
		if (offs == sf::Vector2i({0,0})) continue; // If offset is zero, no change; try next move
		
		dest = enemy_pos + offs; // Add offset to current position

		if (!level_->DoesCollide(enemy_pos, dest)) {
			DEBUG_LogMovement(level_, enemy_pos, std::format("DB{:02}", self_->GetID()));
			return dest;
		}
	}

	// Collision detected
	DEBUG_LogCollision(level_, enemy_pos, dest, std::format("DB{:02}", self_->GetID()));
	return enemy_pos;
}