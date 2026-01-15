#pragma once

#include <SFML/Graphics.hpp>

constexpr int GRID_SIZE = 64;	// 8x8 grid
constexpr int ROW_SIZE = 8;
constexpr int COL_SIZE = 8;
constexpr int TILE_SIZE = 100;

class LevelMap;

class GameObject
{
public:
	GameObject(LevelMap* level, const sf::Vector2i& gridpos) : level_(level), gridpos_(gridpos) {}
	~GameObject() = default;

	// ACCESS
	LevelMap* GetLevelMap() const { return level_; }
	sf::Vector2i GetGridPosition() const { return gridpos_; }
	size_t GetGridIndex() { return (gridpos_.y * 8) + gridpos_.x; }
	void SetLevelMap(LevelMap* level) { level_ = level; }
	void SetGridPosition(sf::Vector2i gridpos) { gridpos_ = gridpos; }

private:
	LevelMap* level_;
	sf::Vector2i gridpos_;
};