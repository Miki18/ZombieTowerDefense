#pragma once
#include "Tower.h"

class CannonTower : public Tower
{
	private:
		sf::Texture* tex_base = nullptr;
		sf::Texture* tex_top = nullptr;
		sf::Vector2f Position;
		int ID;

		const float Scale = 0.75f;

	public:
		CannonTower(sf::Vector2f Pos, sf::Texture* tex1, sf::Texture* tex2, int id);

		void draw(sf::RenderWindow& window) override;
};

