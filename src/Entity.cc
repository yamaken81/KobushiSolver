#include "Entity.h"

#include "LevelMap.h"

Entity::Entity(LevelMap* level, const sf::Vector2i& gridpos, const EntityType& type)
	: GameObject(level, gridpos), type_(type) 
{
	Build();
	Layout();
}

void Entity::Build()
{
	shape_.setSize({ (TILE_SIZE * 3/5), (TILE_SIZE * 3/5) });
	shape_.setOutlineColor(sf::Color::Black);
	shape_.setOutlineThickness(1.0f);
}

void Entity::Layout()
{
	sf::Vector2f gridbounds = level_->GetGridbounds();

	sf::Vector2f size_offset = sf::Vector2f({ TILE_SIZE / 5, TILE_SIZE / 5 });
	sf::Vector2f grid_offset = sf::Vector2f(gridpos_) * (float)TILE_SIZE;
	sf::Vector2f pos = gridbounds + grid_offset + size_offset;

	shape_.setPosition(pos);
}