#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Bullet
{
	public:
		Bullet(sf::Vector2f Position, int M_ID);
		
		void Update(sf::Time time);

	private:
		int MonsterID;
		float velocity = 50;
		sf::Vector2f Pos;
};

