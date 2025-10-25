#include "Tower.h"

Tower::Tower(float hp, float cooldown, float dmg, float radius, float bulletoffset, int id, int price)
{
	this->TowerID = id;
	this->hp = hp;
	this->cooldown = 0;
	this->base_cooldown = cooldown;
	this->dmg = dmg;
	this->radius = radius;
	this->bulletOffset = bulletoffset;
	this->price = price;
}

int Tower::getID()
{
	return TowerID;
}

float Tower::getRadius()
{
	return radius;
}

float Tower::getDmg()
{
	return dmg;
}

sf::Vector2f Tower::getPosition()
{
	return sf::Vector2f();
}

void Tower::draw(sf::RenderWindow& window)
{
}
