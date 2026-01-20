#pragma once

#include "GameObject.h"

#include <iostream>

enum TileType
{
	kEmpty			= 00,
	kStairs			= 10,
	kWater			= 20,
	kWaterSnare		= 21,
	kBarrierGold	= 30
};

enum TileState
{
	kIdle		= 0,
	kActive		= 1,
	kImpassable = 2,
};

class Tile : public GameObject
{
public:
	Tile(LevelMap* level, const sf::Vector2i& gridpos, const TileType& type);
	~Tile() = default;

	// ACCESS
	TileType GetType() const{ return type_; }
	unsigned GetState() const { return state_; }
	std::vector<bool> GetBorders() const { return borders_; }
	void SetType(const TileType& type);
	void SetType();
	void SetState(const TileState& state) { state_ = state; }
	void SetBorders(const std::vector<bool>& borders);
	void SetBorders();
	
	// GAME OBJECT
	void Update(const sf::Time& delta);
	void Render(sf::RenderTarget& target);
	void Layout();

private:
	TileType type_;
	TileState state_;

	std::vector<bool> borders_;			// 0b0000 = NEWS, 0b1010 = N_W_
	sf::RectangleShape rect_;
	sf::VertexArray border_array_;
};