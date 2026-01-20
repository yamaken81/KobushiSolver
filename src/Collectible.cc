#include "Collectible.h"

#include "LevelMap.h"

Collectible::Collectible(LevelMap* level, const sf::Vector2i& gridpos, const CollectibleType& type)
	: GameObject(level, gridpos), type_(type), state_(0)
{
	Build();
	Layout();
}

void Collectible::Render(sf::RenderTarget& target)
{
	if (state_ == 0)
		target.draw(shape_);
}

void Collectible::Build()
{
	float radius = TILE_SIZE * 0.3f;
	shape_.setRadius(radius);
	shape_.setOrigin({ radius, radius });
	shape_.setOutlineColor(sf::Color::Black);
	shape_.setOutlineThickness(1.0f);

	switch (type_)
	{
	case CollectibleType::kFile:
		shape_.setFillColor(sf::Color(128, 128, 128));
		break;
	case CollectibleType::kGem:
		shape_.setFillColor(sf::Color::Blue);
		break;
	}
}

void Collectible::Layout()
{
	sf::Vector2f gridbounds = level_->GetGridbounds();

	sf::Vector2f grid_offset = (sf::Vector2f(gridpos_) + sf::Vector2f(0.5f, 0.5f)) * (float)(TILE_SIZE);
	sf::Vector2f pos = gridbounds + grid_offset;

	shape_.setPosition(pos);
}