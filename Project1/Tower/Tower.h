//Class that conntects all tower classes
#pragma once
#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <iostream>

class Tower
{
	protected:
		const float TileSize = 50.0f;

		float hp;
		float IncreaseHp;

		float cooldown;
		float base_cooldown;
		float IncreaseCooldown;

		float dmg;
		float IncreaseDmg;

		float radius;
		float IncreaseRadius;

		float bulletOffset;
		int TowerID;

		int currentLevel = 1;
		int price;
		int UpgradePrice;
		int IncreaseUpgradePrice;

	public:
		Tower(float hp, float IncHp, float cooldown, float IncCooldown, float dmg, float IncDmg, float radius, float IncRadius, float bulletoffset, int id, int price, int UpgradePrice, int IncUpgradePrice);
		~Tower() {};

		float getRadius();
		float getDmg();
		int getID();
		int getCurrentLevel() { return currentLevel; };

		virtual void Upgrade() {};
		virtual int getUpgradePrice() { return 0; };

		virtual int getSellPrice() { return 0;  };
		virtual void UpdateTower(sf::Time time) {};
		virtual bool CanShoot() { return true; };
		virtual sf::Vector2f getBulletStartingPosition(sf::Vector2f Dir) { return sf::Vector2f(0, 0); };
		virtual sf::Vector2f getPosition();
		virtual sf::Vector2f detectEnemy(sf::Vector2f EnemyPos) { return sf::Vector2f(0,0); };
		virtual void draw(sf::RenderWindow& window);
};

