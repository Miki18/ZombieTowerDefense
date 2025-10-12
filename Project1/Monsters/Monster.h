#pragma once

#include <nlohmann/json.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "../Monsters/PathPoint.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Monster
{
	public:
		Monster(sf::Texture& tex, float hp, float speed, int price, int TileSize, int& P_HP, int& P_Money, std::vector<sf::Vector2i>& starts, std::vector<sf::Vector2i>& ends, std::vector<PathPoints>& paths, int Monster_ID);
		~Monster() {};
		void virtual MonsterUpdate(sf::Time time) {};
		void DrawMonster(sf::RenderWindow& window);
		bool IsDead = false;

		void TakeDamage(float dmg);
		sf::Vector2f getPosition();
		int GetID();

	protected:
		std::vector<sf::Vector2i>* startpoints = nullptr;
		std::vector<sf::Vector2i>* endpoints = nullptr;
		std::vector<PathPoints>* paths = nullptr;

		sf::Texture texture;
		sf::RectangleShape body;

		sf::Vector2f Position;
		//DestinationTile is a Tile's number X and Y (for example tile 0,0; tile 0,1) etc. Map has 18 x 32 tiles
		//Destination is Tile's position on map/screen (for example 0,0 or 0,50) etc. Screen/map has 1600x900 size
		sf::Vector2f Destination;
		sf::Vector2i DestinationTile;
		float speed;
		float health;
		int price;
		int TileSize;
		int& PlayerHP;
		int& PlayerMoney;
		int ID;
		//int dmg and radius only for active monsters

		void virtual MonsterWalk(sf::Time time) {};
		void ChooseDestination();
};

