#include "Collectible.h"

Collectible::Collectible(LevelMap* level, const sf::Vector2i& gridpos, const CollectibleType& type)
	: GameObject(level, gridpos), type_(type), state_(0)
{
	Build();
}

void Collectible::Render(sf::RenderTarget& target, const sf::Vector2f& gridbounds)
{
	if (state_ == 0)
	{
		float xpos = gridbounds.x + (GetGridPosition().x + 0.5f) * TILE_SIZE;
		float ypos = gridbounds.y + (GetGridPosition().y + 0.5f) * TILE_SIZE;
		shape_.setPosition({ xpos, ypos });

		target.draw(shape_);
	}
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


