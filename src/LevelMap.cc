#include "LevelMap.h"

#include <iostream>
#include <fstream>
#include <regex>

void LevelMap::Init()
{
	// RESET LEVEL MAP
	tiles_.clear();
	entities_.clear();
	blocks_.clear();
	collectibles_.clear();
	player_ = nullptr;

	// INIT TILES
	for (int y = 0; y < ROW_SIZE; y++)
		for (int x = 0; x < COL_SIZE; x++)
		{
			Tile tile(this, sf::Vector2i(x, y), TileType::kEmpty);
			tiles_.push_back(std::make_unique<Tile>(tile));
		}

	BuildFromFile(MAP_LOADED);
	// BuildAsDebug(); // Uncomment this line to build a debug map
}

void LevelMap::BuildAsDebug()
{
	if (Tile* ptr = GetTileAt({ 0, 0 }))
	{
		ptr->SetType(TileType::kWaterSnare);
		ptr->SetBorders({ false, false, false, false });
	}
	else
		std::cerr << "ERR: It's not working.\n";

	if (Tile* ptr = GetTileAt({ 0, 1 }))
	{
		ptr->SetType(TileType::kWaterSnare);
		ptr->SetBorders({ false, false, false, false });
	}
	else
		std::cerr << "ERR: It's not working.\n";

	AddEntity({ 1, 0 }, EntityType::kPlayer);
	AddCollectible({ 2, 0 }, CollectibleType::kGem);
}

void LevelMap::BuildFromFile(std::string path)
{
	std::regex regex(R"(\[(\d+),(\d+)\]\s+(\d+)\s+(\d+)\s*(\w*)\s*(\d*))");
	std::smatch match;

	std::ifstream ifs;
	ifs.open(path);
	if (ifs.is_open())
	{
		// Get line from file contents
		std::string line;
		while (std::getline(ifs, line, '\n'))
		{
			// Create match object to parse
			if (std::regex_match(line, match, regex))
				if (match.size() == 7)
				{
					try
					{
						// Set coordinates
						int x(std::stoi(match[1].str()));
						int y(std::stoi(match[2].str()));

						// Set TileType
						TileType tile(static_cast<TileType>(std::stoi(match[4].str())));

						// Set walls
						std::vector<bool> borders(4);
						for (size_t i = 0; i < match[3].str().length(); i++)
						{
							char c = match[3].str()[i];
							if (c == '1')
								borders[i] = true;
							else if (c == '0')
								borders[i] = false;
							else
							{
								borders[i] = false;
								std::cerr << "ERR: Invalid wall bit!\nLine: " << line << "\n";
							}
						}
						
						// Update Tile
						if (Tile* ptr = GetTileAt({ x, y }))
						{
							ptr->SetType(tile);
							ptr->SetBorders(borders);
						}
						else
							std::cerr << "ERR: IT'S NOT WORKING.\n";

						// Add entities and collectibles
						if (match[5].str() == "Entity")
						{
							if (match[6].str() == "00")
								AddEntity({ x, y }, EntityType::kPlayer);
							else
							{
								EntityType entity(static_cast<EntityType>(std::stoi(match[6].str())));
								AddEntity({ x, y }, entity);
							}
						}
						else if (match[5].str() == "Block")
						{
							BlockType block(static_cast<BlockType>(std::stoi(match[6].str())));
							AddBlock({ x, y }, block);
						}

						else if (match[5].str() == "Collectible")
						{
							CollectibleType coll(static_cast<CollectibleType>(std::stoi(match[6].str())));
							AddCollectible({ x, y }, coll);
						}
					}
					catch (const std::invalid_argument& e) {
						std::cerr << "ERR: Could not cast coordinate to int!\nLine: " << line << "\nTraceback: " << e.what() << "\n";
					}
					catch (const std::out_of_range& e) {
						std::cerr << "ERR: Coordinate is out of data type range!\nLine: " << line << "\nTraceback: " << e.what() << "\n";
					}
				}
				else
					std::cerr << "ERR: Invalid number of tokens!\nLine: " << line << "\n";
		}
	}
	else
		std::cerr << "ERR: Could not open map file!";
}

Tile* LevelMap::GetTileAt(const sf::Vector2i& gridpos) const
{
	for (int i = 0; i < tiles_.size(); i++)
		if (tiles_[i]->GetGridPosition() == gridpos)
			return tiles_[i].get();

	return nullptr;
}

Tile* LevelMap::GetTileAt(const size_t& i) const
{
	if (tiles_[i])
		return tiles_[i].get();
	else
		return nullptr;
}

Entity* LevelMap::GetEntityAt(const sf::Vector2i& gridpos)
{
	for (size_t i = 0; i < entities_.size(); i++)
		if (entities_[i]->GetGridPosition() == gridpos)
			return entities_[i].get();
	return nullptr;
}

Entity* LevelMap::GetEntityAt(const size_t& i)
{
	if (entities_[i])
		return entities_[i].get();
	else
		return nullptr;
}

Block* LevelMap::GetBlockAt(const sf::Vector2i& gridpos)
{
	for (size_t i = 0; i < blocks_.size(); i++)
		if (blocks_[i]->GetGridPosition() == gridpos)
			return blocks_[i].get();
	return nullptr;
}

Block* LevelMap::GetBlockAt(const size_t& i)
{
	if (blocks_[i])
		return blocks_[i].get();
	else
		return nullptr;
}

Collectible* LevelMap::GetCollectibleAt(const sf::Vector2i& gridpos)
{
	for (size_t i = 0; i < collectibles_.size(); i++)
		if (collectibles_[i]->GetGridPosition() == gridpos)
			return collectibles_[i].get();
	return nullptr;
}

Collectible* LevelMap::GetCollectibleAt(const size_t& i)
{
	if (entities_[i])
		return collectibles_[i].get();
	else
		return nullptr;
}

void LevelMap::UpdateTile(const sf::Vector2i& gridpos, const TileState& state)
{
	if (Tile* ptr = GetTileAt(gridpos))
		ptr->SetState(state);
	else
		std::cerr << "ERR: Could not get tile at X=" << gridpos.x << ", Y=" << gridpos.y << "\n";
}

void LevelMap::UpdateTile(const size_t& i, const TileState& state)
{
	if (Tile* ptr = GetTileAt(i))
		ptr->SetState(state);
	else
		std::cerr << "ERR: Could not get tile at index " << i << "\n";
}

void LevelMap::AddEntity(const sf::Vector2i& gridpos, const EntityType& type)
{
	if (type == EntityType::kPlayer)
	{
		auto player = std::make_unique<Player>(Player(this, gridpos));
		player_ = player.get();
		entities_.push_back(std::move(player));
		return;
	}
	else
	{
		auto enemy = std::make_unique<Enemy>(Enemy(this, gridpos, type));
		entities_.push_back(std::move(enemy));
		return;
	}
}

void LevelMap::AddBlock(const sf::Vector2i& gridpos, const BlockType& type)
{
	Block block = Block(this, gridpos, type);
	blocks_.push_back(std::make_unique<Block>(block));
}

void LevelMap::AddCollectible(const sf::Vector2i& gridpos, const CollectibleType& type)
{
	Collectible collectible = Collectible(this, gridpos, type);
	collectibles_.push_back(std::make_unique<Collectible>(collectible));
}

void LevelMap::UpdateCollectible(const sf::Vector2i& gridpos)
{
	if (Collectible* ptr = GetCollectibleAt(gridpos))
		ptr->SetState(1);	// The only time you update a collectible's state is when you actually collect it.
	else
		std::cerr << "ERR: Could not get collectible at grid position [" << gridpos.x << "," << gridpos.y << "]\n";
}

void LevelMap::UpdateCollectible(const size_t& i)
{
	if (Collectible* ptr = GetCollectibleAt(i))
		ptr->SetState(1);	// The only time you update a collectible's state is when you actually collect it.
	else
		std::cerr << "ERR: Could not get collectible at index " << i << "\n";
}

void LevelMap::HandleInput(const sf::Event event, const sf::RenderWindow& window)
{
	for (const auto& ptr : entities_)
		ptr->HandleInput(event, window);

	for (const auto& ptr : blocks_)
		ptr->HandleInput(event, window);

#ifdef _DEBUG
	player_->DEBUG_LogMovement();
#endif // _DEBUG
}

void LevelMap::Update(const sf::Time& delta)
{
	for (const auto& ptr : tiles_)
		ptr->Update(delta);

	for (const auto& ptr : entities_)
		ptr->Update(delta);

	for (const auto& ptr : blocks_)
		ptr->Update(delta);

	for (const auto& ptr : collectibles_)
		ptr->Update(delta);

	// Remove blocks that are in the fill state
	blocks_.erase(std::remove_if(blocks_.begin(), blocks_.end(),
		[](const std::unique_ptr<Block>& block)
		{
			return block->GetState() == BlockState::kFill;
		}),
		blocks_.end());
}

void LevelMap::Render(sf::RenderTarget& target, const sf::Vector2f& gridbounds)
{
	for (const auto& ptr : tiles_)
		ptr->Render(target, gridbounds);

	for (const auto& ptr : entities_)
		ptr->Render(target, gridbounds);

	for (const auto& ptr : blocks_)
		ptr->Render(target, gridbounds);

	for (const auto& ptr : collectibles_)
		ptr->Render(target, gridbounds);
}

bool LevelMap::DoesCollide(const sf::Vector2i& origin, const sf::Vector2i& target) const
{
	if (origin.x < 0 || origin.x >= COL_SIZE ||
		origin.y < 0 || origin.y >= ROW_SIZE ||
		target.x < 0 || target.x >= COL_SIZE ||
		target.y < 0 || target.y >= ROW_SIZE)
		return true; // Out of bounds is considered a collision

	int x_diff = target.x - origin.x;
	int y_diff = target.y - origin.y;

	std::vector<bool> target_borders = GetTileAt(target)->GetBorders();
	std::vector<bool> origin_borders = GetTileAt(origin)->GetBorders();

	if (target_borders.empty() || origin_borders.empty())
	{
		std::cerr << "ERR: Borders are empty!\n";
		return true;
	}

	if (target_borders.size() != 4 || origin_borders.size() != 4)
	{
		std::cerr << "ERR: Borders size mismatch! Expected size 4, got " << target_borders.size() << " and " << origin_borders.size() << "\n";
		return true;
	}

	if (x_diff == 0 && y_diff == 0)
	{
		std::cerr << "ERR: No movement detected!\n";
		return true;
	}
	else if (x_diff != 0)
	{
		if (x_diff > 0)		// Player moved right
			return target_borders[3] && origin_borders[1];	// Target's WEST and Origin's EAST
		else if (x_diff < 0)	// Player moved left
			return target_borders[1] && origin_borders[3];	// Target's EAST and Origin's WEST
	}
	else if (y_diff != 0)
	{
		if (y_diff > 0)		// Player moved down
			return target_borders[0] && origin_borders[2];	// Target's NORTH and Origin's SOUTH
		else if (y_diff < 0)	// Player moved up
			return target_borders[2] && origin_borders[0];	// Target's SOUTH and Origin's NORTH
	}
	
	return false; // DoesCollide == false; Movement allowed
}

bool LevelMap::IsPlayerCaught() const
{
	sf::Vector2i player_pos = player_->GetGridPosition();
	for (const auto& entity : entities_)
	{
		if (entity->GetType() == EntityType::kPlayer)
			continue;

		sf::Vector2i entity_pos = entity->GetGridPosition();
		if (player_pos == entity_pos)
			return true;
	}
	return false;
}