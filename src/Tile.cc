#include "Tile.h"

#include "LevelMap.h"

#include <iostream>

Tile::Tile(LevelMap* level, const sf::Vector2i& gridpos, const TileType& type)
	: GameObject(level, gridpos), type_(type), state_(TileState::kIdle)
{
	// TILE
	rect_.setSize({ TILE_SIZE, TILE_SIZE });

	// WALLS
	borders_ = { false, false, false, false };

	border_array_.setPrimitiveType(sf::PrimitiveType::Lines);
	border_array_.resize(8);

	for (size_t i = 0; i < border_array_.getVertexCount(); i++)
		border_array_[i].color = sf::Color::Black;

	SetType();
	SetBorders();
	Layout();
}

void Tile::SetType(const TileType& type)
{
	type_ = type;
	SetType();
}

void Tile::SetType()
{
	switch (type_)
	{
	case TileType::kStairs:
		rect_.setFillColor(sf::Color(165, 157, 132));
		break;
	case TileType::kWater:
		state_ = TileState::kImpassable;
		rect_.setFillColor(sf::Color(0, 101, 248));
		break;
	case TileType::kWaterSnare:
		rect_.setFillColor(sf::Color(0, 202, 255));
		break;
	case TileType::kEmpty:
		state_ = TileState::kIdle;
		rect_.setFillColor(sf::Color::White);
		break;
	default:
		rect_.setFillColor(sf::Color::Green);
	}
}

void Tile::SetBorders(const std::vector<bool>& borders)
{
	borders_ = borders;
	SetBorders();
}

void Tile::SetBorders()
{
	if (!borders_[0])
	{
		border_array_[0].color = sf::Color::Transparent;
		border_array_[1].color = sf::Color::Transparent;
	}
	else
	{
		border_array_[0].color = sf::Color::Black;
		border_array_[1].color = sf::Color::Black;
	}
	if (!borders_[1])
	{
		border_array_[2].color = sf::Color::Transparent;
		border_array_[3].color = sf::Color::Transparent;
	}
	else
	{
		border_array_[2].color = sf::Color::Black;
		border_array_[3].color = sf::Color::Black;
	}
	if (!borders_[2])
	{
		border_array_[4].color = sf::Color::Transparent;
		border_array_[5].color = sf::Color::Transparent;
	}
	else
	{
		border_array_[4].color = sf::Color::Black;
		border_array_[5].color = sf::Color::Black;
	}
	if (!borders_[3])
	{
		border_array_[6].color = sf::Color::Transparent;
		border_array_[7].color = sf::Color::Transparent;
	}
	else
	{
		border_array_[6].color = sf::Color::Black;
		border_array_[7].color = sf::Color::Black;
	}
}

void Tile::Update(const sf::Time& delta)
{
	LevelMap* level = GetLevelMap();
	Player* player = level->GetPlayer();

	// IDLE/ACTIVE CHECK
	if (state_ == TileState::kIdle && 
		player->GetGridPosition() == gridpos_)
		state_ = TileState::kActive;
	else if (state_ == TileState::kActive &&
		player->GetGridPosition() != gridpos_)
	{
		if (type_ == TileType::kWaterSnare)
			SetType(TileType::kWater);
		else
			state_ = TileState::kIdle;
	}
}

void Tile::Render(sf::RenderTarget& target)
{
	target.draw(rect_);
	target.draw(border_array_);
}

void Tile::Layout()
{
	// THE RECTANGLE
	sf::Vector2f gridbounds = level_->GetGridbounds();

	sf::Vector2f grid_offset = sf::Vector2f(gridpos_ * TILE_SIZE);
	sf::Vector2f pos = gridbounds + grid_offset;
	rect_.setPosition(pos);

	// THE BORDERS: This is disgusting but it works
	border_array_[0].position = sf::Vector2f({ pos.x, pos.y });							// NW
	border_array_[1].position = sf::Vector2f({ pos.x + TILE_SIZE, pos.y });				// NE
	border_array_[2].position = sf::Vector2f({ pos.x + TILE_SIZE, pos.y });				// NE
	border_array_[3].position = sf::Vector2f({ pos.x + TILE_SIZE, pos.y + TILE_SIZE });	// SE
	border_array_[4].position = sf::Vector2f({ pos.x + TILE_SIZE, pos.y + TILE_SIZE });	// SE
	border_array_[5].position = sf::Vector2f({ pos.x, pos.y + TILE_SIZE });				// SW
	border_array_[6].position = sf::Vector2f({ pos.x, pos.y + TILE_SIZE });				// SW
	border_array_[7].position = sf::Vector2f({ pos.x, pos.y });							// NW
}