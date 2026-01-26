#include "LevelMap.h"

#include <iostream>
#include <fstream>
#include <regex>

// Helper enum for collision detection.
enum class Direction { North = 0, East = 1, South = 2, West = 3 };

Direction GetOpposite(Direction d)
{
	return static_cast<Direction>((static_cast<int>(d) + 2) % 4);
}

void LevelMap::Init(int level)
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

	std::string path = "res/lv" + std::to_string(level) + ".map";
	BuildFromFile(path);
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
	std::ifstream ifs(path);
	if (!ifs.is_open()) {
		std::cerr << "ERR: Could not open map file!";
		return;
	}

	std::regex regex(R"(\[(\d+),(\d+)\]\s+(\d+)\s+(\d+)\s*(\w*)\s*(\d*))");
	std::smatch match;
	std::string line;

	while (std::getline(ifs, line)) {
		// Guard clause: skip empty/whitespace lines
		if (line.empty() || std::all_of(line.begin(), line.end(), isspace)) {
			continue;
		}

		// Guard clause: regex match integrity
		if (!std::regex_match(line, match, regex) || match.size() != 7) {
			std::cerr << "ERR: Invalid format! Line: " << line << "\n";
			continue;
		}

		try {
			// Extract data from regex match
			int x = std::stoi(match[1].str());
			int y = std::stoi(match[2].str());
			std::string wall_mask = match[3].str();
			TileType tile_type = static_cast<TileType>(std::stoi(match[4].str()));

			// Guard clause: pointer check
			Tile* ptr = GetTileAt({ x, y });
			if (!ptr) {
				std::cerr << "ERR: Tile OOB at " << x << "," << y << "\n";
				continue;
			}

			// Handle borders
			std::vector<bool> borders(4, false);
			for (size_t i = 0; i < 4 && i < wall_mask.length(); ++i) {
				borders[i] = (wall_mask[i] == '1');	// Return true if char is '1'. True means a wall exists on that side.
			}

			ptr->SetType(tile_type);
			ptr->SetBorders(borders);

			// Populate level with entities/blocks/collectibles
			std::string category = match[5].str();
			std::string sub_type = match[6].str();

			// Skip if no entity/block
			if (category.empty()) continue;

			if (category == "Entity") {
				// Check for player "00" or parse ID
				EntityType entity_type = (sub_type == "00") ? EntityType::kPlayer : static_cast<EntityType>(std::stoi(sub_type));
				AddEntity({ x, y }, entity_type);
			}
			else if (category == "Block")
				AddBlock({ x, y }, static_cast<BlockType>(std::stoi(sub_type)));
			else if (category == "Collectible")
				AddCollectible({ x, y }, static_cast<CollectibleType>(std::stoi(sub_type)));
		}
		catch (const std::exception& e) {
			std::cerr << "ERR: Parsing failed: " << e.what() << " | Line: " << line << "\n";
		}
	}
#ifdef _DEBUG
	std::cerr << "INFO: Map \"" << MAP_LOADED << "\" loaded.\n\n";
#endif
}

sf::Vector2f LevelMap::GetGridbounds() const
{
	sf::Vector2f window_size = window_.GetWindowSize();
	sf::Vector2f center_offset = sf::Vector2f({ (TILE_SIZE * ROW_SIZE), (TILE_SIZE * COL_SIZE) });
	sf::Vector2f gridbounds = (window_size - center_offset) / 2.0f;

	return gridbounds;
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
		auto player = std::make_unique<Player>(this, gridpos);
		player_ = player.get();
		entities_.push_back(std::move(player));
		return;
	}
	else
	{
		auto enemy = std::make_unique<Enemy>(this, gridpos, type);
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

void LevelMap::HandleInput(const sf::Event event)
{
	for (const auto& ptr : entities_)
		ptr->HandleInput(event);

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
		ptr->Render(target);

	for (const auto& ptr : entities_)
		ptr->Render(target);

	for (const auto& ptr : blocks_)
		ptr->Render(target);

	for (const auto& ptr : collectibles_)
		ptr->Render(target);
}

bool LevelMap::DoesCollide(const sf::Vector2i& origin, const sf::Vector2i& target) const
{
	// Helper: Check if out of bounds
	auto is_oob = [](const sf::Vector2i& p) {
		return p.x < 0 || p.x >= COL_SIZE || p.y < 0 || p.y >= ROW_SIZE; };

	if (is_oob(origin) || is_oob(target)) return true;

	// Determine direction
	sf::Vector2i diff = target - origin;
	Direction dir;

	if (diff.x > 0) dir = Direction::East;
	else if (diff.x < 0) dir = Direction::West;
	else if (diff.y > 0) dir = Direction::South;
	else if (diff.y < 0) dir = Direction::North;
	else return true; // No movement

	// Get tile data
	const auto& origin_borders = GetTileAt(origin)->GetBorders();
	const auto& target_borders = GetTileAt(target)->GetBorders();

	// Guard clause: border data integrity
	if (origin_borders.size() < 4 || target_borders.size() < 4) return true;

	// If border exists (true), can't leave/enter (false)
	bool can_leave = !origin_borders[static_cast<int>(dir)];
	bool can_enter = !target_borders[static_cast<int>(GetOpposite(dir))];

	// If can't leave/enter (false), collision occurs (true)
	return !can_leave || !can_enter;
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