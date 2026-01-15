#include "Entity.h"

Entity::Entity(LevelMap* level, const sf::Vector2i& gridpos, const EntityType& type)
	: GameObject(level, gridpos), type_(type) 
{
	Build();
}

void Entity::Render(sf::RenderTarget& target, const sf::Vector2f& gridbounds)
{
	float xpos = gridbounds.x + (GetGridPosition().x * TILE_SIZE) + (TILE_SIZE / 5);
	float ypos = gridbounds.y + (GetGridPosition().y * TILE_SIZE) + (TILE_SIZE / 5);
	shape_.setPosition({ xpos, ypos });

	target.draw(shape_);
}

void Entity::Build()
{
	shape_.setSize({ (TILE_SIZE * 3/5), (TILE_SIZE * 3/5) });
	shape_.setOutlineColor(sf::Color::Black);
	shape_.setOutlineThickness(1.0f);
}