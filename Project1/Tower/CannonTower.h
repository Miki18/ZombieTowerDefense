#pragma once
#include "Tower.h"

class CannonTower : public Tower
{
	private:
		sf::Sprite base;
		sf::Sprite top;
		sf::CircleShape Dot;

	public:
		CannonTower(sf::Vector2f Pos, float hp, float IncHp, float cooldown, float IncCooldown, float dmg, float IncDmg, float radius, float IncRadius, float bulletoffset, float bulletspeed, sf::Texture* tex1, sf::Texture* tex2, int id, int price, int UPrice, int IncUPrice, sf::SoundBuffer& tower_shoot);

		void Upgrade() override;
		int getUpgradePrice() override;
		int getSellPrice() override;
		int getRepairPrice() override;

		float getBulletSpeed() override;
		bool CanShoot() override;
		sf::Vector2f getBulletStartingPosition(sf::Vector2f Dir) override;
		void UpdateTower(sf::Time time) override;
		sf::Vector2f getPosition() override;
		sf::Vector2f detectEnemy(sf::Vector2f EnemyPos) override;
		void draw(sf::RenderWindow& window) override;
};

