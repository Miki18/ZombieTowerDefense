#pragma once
#include "Tower.h"

class CannonTower : public Tower
{
	private:
		sf::Sprite base;
		sf::Sprite top;

		sf::Vector2f Position;
		int ID;

	public:
		CannonTower(sf::Vector2f Pos, sf::Texture* tex1, sf::Texture* tex2, int id);

		void draw(sf::RenderWindow& window) override;
};

