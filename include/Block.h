#pragma once

#include "GameObject.h"

enum BlockType
{
	kPlain	= 0x00
};

enum BlockState
{
	kNormal	= 0x0,
	kFill	= 0x1
};

class Block : public GameObject
{
public:
	Block(LevelMap* level, const sf::Vector2i& gridpos, const BlockType& type);

	// ACCESS
	BlockType GetType() { return type_; }
	void SetType(BlockType type) { type_ = type; }
	BlockState GetState() { return state_; }
	void SetState(BlockState state) { state_ = state; }

	// GAME OBJECT
	void HandleInput(const sf::Event event, const sf::RenderWindow& window) {};
	void Update(const sf::Time& delta);
	void Render(sf::RenderTarget& target, const sf::Vector2f& gridbounds);

	void Build();

private:
	BlockType type_;
	BlockState state_;
	std::vector<bool> is_pushable_; // NESW
	bool is_adjacent = false; // If the player is adjacent to the block

	sf::RectangleShape shape_;
};