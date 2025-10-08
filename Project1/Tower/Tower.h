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
		float dmg;
		float radius;
		sf::Vector2f bulletPoint;
		int TowerID;

	public:
		Tower(float hp, float cooldown, float dmg, float radius, sf::Vector2f bulletpoint, int id);
		~Tower() {};

		int getID();
		float getRadius();
		virtual void draw(sf::RenderWindow& window);
};

