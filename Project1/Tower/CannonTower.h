#pragma once
#include "Tower.h"

class CannonTower : public Tower
{
	private:
		sf::Sprite base;
		sf::Sprite top;

		sf::Vector2f Position;

	public:
		CannonTower(sf::Vector2f Pos, float hp, float cooldown, float dmg, float radius, sf::Vector2f bulletpoints, sf::Texture* tex1, sf::Texture* tex2, int id);


		void draw(sf::RenderWindow& window) override;
};

