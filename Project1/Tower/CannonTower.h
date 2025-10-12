#pragma once
#include "Tower.h"

class CannonTower : public Tower
{
	private:
		sf::Sprite base;
		sf::Sprite top;

		sf::Vector2f Position;

	public:
		CannonTower(sf::Vector2f Pos, float hp, float cooldown, float dmg, float radius, float bulletoffset, sf::Texture* tex1, sf::Texture* tex2, int id);

		bool CanShoot() override;
		sf::Vector2f getBulletStartingPosition(sf::Vector2f Dir) override;
		void UpdateTower(sf::Time time) override;
		sf::Vector2f getPosition() override;
		sf::Vector2f detectEnemy(sf::Vector2f EnemyPos) override;
		void draw(sf::RenderWindow& window) override;
};

