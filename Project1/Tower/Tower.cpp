#include "Tower.h"

Tower::Tower(sf::Vector2f Position, float hp, float IncHp, float cooldown, float IncCooldown, float dmg, float IncDmg, float radius, float IncRadius, float bulletoffset, float bulletspeed, int id, int price, int UPrice, int IncUpgradePrice)
{
	this->Position = Position;

	this->TowerID = id;

	this->maxhp = hp;
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
	this->bulletSpeed = bulletspeed;

	this->price = price;
	this->UpgradePrice = UPrice;
	this->IncreaseUpgradePrice = IncUpgradePrice;
}

int Tower::getID()
{
	return TowerID;
}

bool Tower::hasMaxHeath()
{
	if (hp == maxhp)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Tower::getRadius()
{
	return radius;
}

float Tower::getDmg()
{
	return dmg;
}

void Tower::TakeDmg(int Damage)
{
	hp = hp - Damage;
}

sf::Vector2f Tower::getPosition()
{
	return sf::Vector2f();
}

void Tower::draw(sf::RenderWindow& window)
{
}

void Tower::drawhealth(sf::RenderWindow& window)
{
	if (hp / maxhp < 1)
	{
		sf::Vector2f HealthbarSize = { 20, 5 };

		sf::RectangleShape back(HealthbarSize);
		back.setFillColor(sf::Color(sf::Color::Black));
		back.setPosition(sf::Vector2f(Position.x-HealthbarSize.x/2, Position.y-HealthbarSize.y/2));

		sf::RectangleShape fill(sf::Vector2f(HealthbarSize.x * hp / maxhp, HealthbarSize.y));
		fill.setFillColor(sf::Color(sf::Color::Red));
		fill.setPosition(sf::Vector2f(sf::Vector2f(Position.x-HealthbarSize.x/2, Position.y- HealthbarSize.y / 2)));

		window.draw(back);
		window.draw(fill);
	}
}
