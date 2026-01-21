#pragma once

#include "GameObject.h"

enum CollectibleType
{
	kFile		= 00,
	kGem		= 01,
	kKeyGold	= 10,
};

class Collectible : public GameObject
{
public:
	Collectible(LevelMap* level, const sf::Vector2i& gridpos, const CollectibleType& type);

	// ACCESS
	CollectibleType GetType() { return type_; }
	void SetType(CollectibleType type) { type_ = type; }
	unsigned GetState() { return state_; }
	void SetState(const unsigned& state) { state_ = state; }

	// GAME OBJECT
	void Update(const sf::Time& delta) {};
	void Render(sf::RenderTarget& target);

	void Build();
	void Layout();

private:
	CollectibleType type_;
	unsigned state_;

	sf::CircleShape shape_;
};