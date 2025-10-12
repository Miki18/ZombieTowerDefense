#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Bullet
{
	public:
		Bullet(sf::Vector2f Position, float damage, sf::Vector2f TargetCurrentPos, int Target_ID);
		
		float getDmg();
		float getRadius();
		int getTargetID();
		sf::Vector2f getPosition();
		sf::Vector2f getTargetPos();

		void DrawBullet(sf::RenderWindow& window);
		void Update(sf::Time time, sf::Vector2f Dir, sf::Vector2f TargetCurrentPos);

	private:
		sf::Vector2f TargetPos;
		sf::CircleShape circle;
		int TargetID;
		float radius = 0;
		float velocity = 200;
		float dmg;
};