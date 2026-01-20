#include "Block.h"

#include "LevelMap.h"

Block::Block(LevelMap* level, const sf::Vector2i& gridpos, const BlockType& type)
	: GameObject(level, gridpos), type_(type), state_(BlockState::kNormal), 
	  is_pushable_({ false, false, false, false })
{
	Build();
	Layout();
}

void Block::Update(const sf::Time& delta)
{
	LevelMap* level = GetLevelMap();

	is_pushable_ = { false, false, false, false }; // Reset pushable states

	// Block push logic
	if (state_ != BlockState::kFill)
	{
		// Check if the block can be pushed in each direction	
		if (level->DoesCollide(GetGridPosition(), GetGridPosition() + sf::Vector2i(0, -1)))
			is_pushable_[0] = true; // North
		if (level->DoesCollide(GetGridPosition(), GetGridPosition() + sf::Vector2i(1, 0)))
			is_pushable_[1] = true; // East
		if (level->DoesCollide(GetGridPosition(), GetGridPosition() + sf::Vector2i(0, 1)))
			is_pushable_[2] = true; // South
		if (level->DoesCollide(GetGridPosition(), GetGridPosition() + sf::Vector2i(-1, 0)))
			is_pushable_[3] = true; // West

		// Check if the player is adjacent to the block
		sf::Vector2i player_pos = level->GetPlayer()->GetGridPosition();
		if (player_pos == GetGridPosition() + sf::Vector2i({ 0, -1 }) ||
			player_pos == GetGridPosition() + sf::Vector2i({ 1,  0 }) ||
			player_pos == GetGridPosition() + sf::Vector2i({ 0,  1 }) ||
			player_pos == GetGridPosition() + sf::Vector2i({ -1,  0 }))
			is_adjacent = true;
		else
			is_adjacent = false;
	}

	// Handle specific block type logic
	Tile* tile = level->GetTileAt(GetGridPosition());
	if ((tile->GetType() == TileType::kWater) && (state_ != BlockState::kFill))
	{
		tile->SetType(TileType::kEmpty);
		SetState(BlockState::kFill);
	}
}

void Block::Build()
{
	shape_.setSize({ (TILE_SIZE * 3 / 5), (TILE_SIZE * 3 / 5) });
	shape_.setOutlineColor(sf::Color::Black);
	shape_.setOutlineThickness(1.0f);

	switch (type_)
	{
	case BlockType::kPlain:
		shape_.setFillColor(sf::Color(221, 218, 208));
		break;
	}
}

void Block::Layout()
{
	sf::Vector2f gridbounds = level_->GetGridbounds();

	sf::Vector2f size_offset = sf::Vector2f({ TILE_SIZE / 5, TILE_SIZE / 5 });
	sf::Vector2f grid_offset = sf::Vector2f(gridpos_) * (float)TILE_SIZE;
	sf::Vector2f pos = gridbounds + grid_offset + size_offset;

	shape_.setPosition(pos);
}