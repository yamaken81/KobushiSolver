#pragma once

#include "StateManager.h"
#include "Tile.h"
#include "Block.h"
#include "Collectible.h"
#include "Player.h"
#include "Enemy.h"

#include <vector>

const std::string MAP_LOADED = "res/lv4.map";

class LevelMap
{
public:
	LevelMap(WindowContext& window)
		: is_enemyturn(false), player_(nullptr), window_(window) {}

	// CONSTRUCTOR HELPERS
	void Init(int level);
	void BuildAsDebug();
	void BuildFromFile(std::string path);

	// ACCESS
	WindowContext& GetWindowContext() const { return window_; }
	sf::Vector2f GetGridbounds() const;

	bool IsEnemyTurn() const { return is_enemyturn; }
	void SetEnemyTurn(bool flag) { is_enemyturn = flag; }

	Tile* GetTileAt(const sf::Vector2i& gridpos) const;
	Tile* GetTileAt(const size_t& i) const;
	Player* GetPlayer() { return player_; }
	Entity* GetEntityAt(const sf::Vector2i& gridpos);
	Entity* GetEntityAt(const size_t& i);
	Block* GetBlockAt(const sf::Vector2i& gridpos);
	Block* GetBlockAt(const size_t& i);
	Collectible* GetCollectibleAt(const sf::Vector2i& gridpos);
	Collectible* GetCollectibleAt(const size_t& i);

	// GRID OBJECTS
	void UpdateTile(const sf::Vector2i& gridpos, const TileState& state);
	void UpdateTile(const size_t&, const TileState& state);
	void AddEntity(const sf::Vector2i& gridpos, const EntityType& type);
	void AddBlock(const sf::Vector2i& gridpos, const BlockType& type);
	void AddCollectible(const sf::Vector2i& gridpos, const CollectibleType& type);
	void UpdateCollectible(const sf::Vector2i& gridpos);
	void UpdateCollectible(const size_t& i);

	// GAME OBJECT
	void HandleInput(const sf::Event event);
	void Update(const sf::Time& delta);
	void Render(sf::RenderTarget& target, const sf::Vector2f& gridbounds);

	// LEVEL MAP
	bool DoesCollide(const sf::Vector2i& origin, const sf::Vector2i& target) const;
	bool IsPlayerCaught() const;

private:
	WindowContext& window_;

	bool is_enemyturn;

	std::vector<std::unique_ptr<Tile>> tiles_;
	std::vector<std::unique_ptr<Entity>> entities_;
	std::vector<std::unique_ptr<Block>> blocks_;
	std::vector<std::unique_ptr<Collectible>> collectibles_;

	Player* player_;
};