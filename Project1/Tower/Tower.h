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

		float maxhp;
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
		float bulletSpeed;
		int TowerID;

		int currentLevel = 1;
		int price;
		int UpgradePrice;
		int IncreaseUpgradePrice;

		sf::Vector2f Position;

		sf::Sound sound;

	public:
		Tower(sf::Vector2f Position, float hp, float IncHp, float cooldown, float IncCooldown, float dmg, float IncDmg, float radius, float IncRadius, float bulletoffset, float bulletspeed, int id, int price, int UpgradePrice, int IncUpgradePrice, sf::SoundBuffer& tower_shoot);
		~Tower() {};

		float getRadius();
		float getDmg();
		int getID();
		int getCurrentLevel() { return currentLevel; };
		int gethp() { return hp; };
		bool hasMaxHeath();

		virtual void Upgrade() {};
		virtual int getUpgradePrice() { return 0; };

		virtual float getBulletSpeed() { return 0.0f; }
		virtual int getSellPrice() { return 0;  };
		virtual int getRepairPrice() { return 0; };
		void Repair() { hp = maxhp; };
		virtual void UpdateTower(sf::Time time) {};
		virtual void GenerateGold(int* Money) {};
		virtual bool CanShoot() { return false; };
		virtual void TakeDmg(int Damage);
		virtual sf::Vector2f getBulletStartingPosition(sf::Vector2f Dir) { return sf::Vector2f(0, 0); };
		virtual sf::Vector2f getPosition();
		virtual sf::Vector2f detectEnemy(sf::Vector2f EnemyPos) { return sf::Vector2f(0,0); };
		virtual void draw(sf::RenderWindow& window);
		void drawhealth(sf::RenderWindow& window);
		void PlaySound();
		void SetVolume(float vol);
};

