#include "Tower.h"

Tower::Tower(float hp, float IncHp, float cooldown, float IncCooldown, float dmg, float IncDmg, float radius, float IncRadius, float bulletoffset, int id, int price, int UPrice, int IncUpgradePrice)
{
	this->TowerID = id;

	this->hp = hp;
	this->IncreaseHp = IncHp;

	this->cooldown = 0;
	this->base_cooldown = cooldown;
	this->IncreaseCooldown = IncCooldown;

	this->dmg = dmg;
	this->IncreaseDmg = IncDmg;

	this->radius = radius;
	this->IncreaseRadius = IncRadius;

	this->bulletOffset = bulletoffset;

	this->price = price;
	this->UpgradePrice = UPrice;
	this->IncreaseUpgradePrice = IncUpgradePrice;
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
