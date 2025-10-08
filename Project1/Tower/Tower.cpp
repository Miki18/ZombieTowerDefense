#include "Tower.h"

Tower::Tower(float hp, float cooldown, float dmg, float radius, sf::Vector2f bulletpoint, int id)
{
	this->TowerID = id;
	this->hp = hp;
	this->cooldown = cooldown;
	this->dmg = dmg;
	this->radius = radius;
	this->bulletPoint = bulletpoint;
}

int Tower::getID()
{
	return TowerID;
}

float Tower::getRadius()
{
	return radius;
}

void Tower::draw(sf::RenderWindow& window)
{
}
