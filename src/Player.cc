#include "Player.h"

#include "LevelMap.h"

Player::Player(LevelMap* level, const sf::Vector2i& gridpos)
	: Entity(level, gridpos, EntityType::kPlayer) 
{
	Build();
}

void Player::Update(const sf::Time& delta)
{
	LevelMap* level = GetLevelMap();
	Collectible* player_tile = level->GetCollectibleAt(GetGridPosition());

	if (player_tile == nullptr)
		return;

	if (player_tile->GetType() == CollectibleType::kGem)
	{
		if (player_tile->GetState() == 0)
		{
			gems_++;
			player_tile->SetState(1); // Collect the gem
			level->UpdateCollectible(GetGridPosition());
			std::cout << "Collected a gem!\n";
		}
	}
}

void Player::HandleInput(const sf::Event event)
{
	if (const auto* key = event.getIf<sf::Event::KeyPressed>())
	{
		sf::Vector2i new_pos = GetGridPosition();
		sf::Vector2i new_block_pos = new_pos; // Position for the block to be pushed

		switch (key->code)
		{
		case sf::Keyboard::Key::Up:
			new_pos.y -= 1;
			new_block_pos.y -= 2;
			break;
		case sf::Keyboard::Key::Down:
			new_pos.y += 1;
			new_block_pos.y += 2;
			break;
		case sf::Keyboard::Key::Left:
			new_pos.x -= 1;
			new_block_pos.x -= 2;
			break;
		case sf::Keyboard::Key::Right:
			new_pos.x += 1;
			new_block_pos.x += 2;
			break;
		}

		// Check if position should be updated
		LevelMap* level = GetLevelMap();
		bool in_row_bounds = new_pos.x >= 0 && new_pos.x < ROW_SIZE;
		bool in_col_bounds = new_pos.y >= 0 && new_pos.y < COL_SIZE;
		bool no_collision = !level->DoesCollide(GetGridPosition(), new_pos);
		bool is_tile_passable = false;
		bool is_blockpath_clear = false;

		Tile* tile = level->GetTileAt(new_pos);
		if (tile)
			if (tile->GetState() != TileState::kImpassable)
				is_tile_passable = true;

		Block* block = level->GetBlockAt(new_pos);
		if (block)
		{
			if (block->GetState() != BlockState::kFill)
			{
				if (!level->DoesCollide(block->GetGridPosition(), new_block_pos))
					is_blockpath_clear = true;
			}
			else
				is_blockpath_clear = true; // Block fills a water tile, so path is clear
		}
		else
			is_blockpath_clear = true; // No block at the new position, so path is clear

		// Check if all flags are set
		if (in_row_bounds && in_col_bounds && no_collision && is_tile_passable && is_blockpath_clear)
		{
			if (block)
				block->SetGridPosition(new_block_pos);

			SetGridPosition(new_pos);
			Layout();
			level->SetEnemyTurn(true); // Set enemy turn to true after player moves
		}
#ifdef _DEBUG
		else
			DEBUG_LogCollision(new_pos);
#endif
	}
}

void Player::Build()
{
	sf::RectangleShape shape = GetShape();
	shape.setFillColor(sf::Color::Magenta);
	SetShape(shape);
}

void Player::DEBUG_LogMovement()
{
	LevelMap* level = GetLevelMap();
	const Tile* tile_at_player = level->GetTileAt(gridpos_);
	std::cout <<
		"PLAYER: [" << tile_at_player->GetGridPosition().x << "," << tile_at_player->GetGridPosition().y <<
		"] Type: " << tile_at_player->GetType() <<
		", Borders: " << tile_at_player->GetBorders()[0] << tile_at_player->GetBorders()[1] <<
		tile_at_player->GetBorders()[2] << tile_at_player->GetBorders()[3] << "\n";
}

void Player::DEBUG_LogCollision(const sf::Vector2i& new_pos)
{
	LevelMap* level = GetLevelMap();
	if (level->DoesCollide(gridpos_, new_pos))
		std::cout << "PLAYER: Collision detected! Movement blocked.\n";
}