#pragma once
#include "Tower.h"

class Goldmine : public Tower
{
	private:
		sf::Sprite base;
		sf::Sprite top;

	public:
		Goldmine(sf::Vector2f Pos, float hp, float IncHp, float cooldown, float IncCooldown, float dmg, float IncDmg, float radius, float IncRadius, float bulletoffset, float bulletspeed, sf::Texture* tex1, sf::Texture* tex2, int id, int price, int UPrice, int IncUPrice, sf::SoundBuffer& tower_shoot);

		void Upgrade() override;
		int getUpgradePrice() override;
		int getSellPrice() override;
		int getRepairPrice() override;
		void GenerateGold(int* Money) override;
		void UpdateTower(sf::Time time) override;
		sf::Vector2f getPosition() override;
		void draw(sf::RenderWindow& window) override;
};

