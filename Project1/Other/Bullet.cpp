#include "Bullet.h"

Bullet::Bullet(sf::Vector2f Position, float damage, float Speed, sf::Vector2f TargetCurrentPos, int Target_ID, float Radius, bool Friendly, bool Permanent, bool Explosive)
{
	TargetID = Target_ID;
	dmg = damage;
	TargetPos = TargetCurrentPos;
	velocity = Speed;
	IsFriendly = Friendly;
	sf::Vector2f Dir = TargetCurrentPos - Position;
	float length = Dir.length();
	Dir.x = Dir.x / length;
	Dir.y = Dir.y / length;
	SavedDir = Dir;

	circle.setOrigin(sf::Vector2f(3, 3));
	circle.setRadius(Radius);
	circle.setOutlineThickness(1);
	circle.setPosition(Position);
	if (IsFriendly == true)
	{
		circle.setOutlineColor(sf::Color::Red);
		circle.setFillColor(sf::Color(200, 80, 80, 255));
	}
	else
	{
		circle.setOutlineColor(sf::Color(50, 20, 50, 255));
		circle.setFillColor(sf::Color(150, 60, 150, 255));
	}

	IsPermanent = Permanent;
	IsExplosive = Explosive;

	this->radius = Radius;
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

void Bullet::AddMonsterID(int ID)
{
	MonstersID.push_back(ID);
}

bool Bullet::CheckMonsterID(int ID)
{
	if (count(MonstersID.begin(), MonstersID.end(), ID) > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
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

void Bullet::Update(sf::Time time, sf::Vector2f TargetCurrentPos)
{
	circle.setPosition(circle.getPosition() + SavedDir * velocity * time.asSeconds());
}
