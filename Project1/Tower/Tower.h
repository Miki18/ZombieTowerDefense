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
		float cooldown;
		float base_cooldown;
		float dmg;
		float radius;
		float bulletOffset;
		int TowerID;

	public:
		Tower(float hp, float cooldown, float dmg, float radius, float bulletoffset, int id);
		~Tower() {};

		int getID();

		float getRadius();
		float getDmg();
		virtual void UpdateTower(sf::Time time) {};
		virtual bool CanShoot() { return true; };
		virtual sf::Vector2f getBulletStartingPosition(sf::Vector2f Dir) { return sf::Vector2f(0, 0); };
		virtual sf::Vector2f getPosition();
		virtual sf::Vector2f detectEnemy(sf::Vector2f EnemyPos) { return sf::Vector2f(0,0); };
		virtual void draw(sf::RenderWindow& window);
};

