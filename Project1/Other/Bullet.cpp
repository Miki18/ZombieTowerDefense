#include "Bullet.h"

Bullet::Bullet(sf::Vector2f Position, float damage, float Speed, sf::Vector2f TargetCurrentPos, int Target_ID)
{
	TargetID = Target_ID;
	dmg = damage;
	TargetPos = TargetCurrentPos;
	velocity = Speed;

	circle.setOrigin(sf::Vector2f(3, 3));
	circle.setRadius(5);
	circle.setOutlineThickness(1);
	circle.setPosition(Position);
	circle.setOutlineColor(sf::Color::Red);
	circle.setFillColor(sf::Color(200, 80, 80, 255));

	radius = 3;
}

float Bullet::getDmg()
{
	return dmg;
}

float Bullet::getRadius()
{
	return radius;
}

int Bullet::getTargetID()
{
	return TargetID;
}

sf::Vector2f Bullet::getPosition()
{
	return circle.getPosition();
}

sf::Vector2f Bullet::getTargetPos()
{
	return TargetPos;
}

void Bullet::DrawBullet(sf::RenderWindow& window)
{
	window.draw(circle);
}

void Bullet::Update(sf::Time time, sf::Vector2f Dir, sf::Vector2f TargetCurrentPos)
{
	circle.setPosition(circle.getPosition() + Dir * velocity * time.asSeconds());
	TargetPos = TargetCurrentPos;
}
