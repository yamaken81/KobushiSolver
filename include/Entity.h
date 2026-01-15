#pragma once

#include "GameObject.h"

enum EntityType
{
	kPlayer		= 00,
	kDevilbot	= 10	// 2SQ, VERTICALLY THEN HORIZONTALLY
};

class Entity : public GameObject
{
public:
	Entity(LevelMap* level, const sf::Vector2i& gridpos, const EntityType& type);

	// ACCESS
	EntityType GetType() { return type_; }
	void SetType(EntityType type) { type_ = type; }
	sf::RectangleShape GetShape() { return shape_; }
	void SetShape(const sf::RectangleShape& shape) { shape_ = shape; }

	// GAME OBJECT
	virtual void HandleInput(const sf::Event event, const sf::RenderWindow& window) {};
	virtual void Update(const sf::Time& delta) {};
	virtual void Render(sf::RenderTarget& target, const sf::Vector2f& gridbounds);

	virtual void Build();
	
private:
	EntityType type_;

	sf::RectangleShape shape_;
};