#include "Goldmine.h"

Goldmine::Goldmine(sf::Vector2f Pos, float hp, float IncHp, float cooldown, float IncCooldown, float dmg, float IncDmg, float radius, float IncRadius, float bulletoffset, float bulletspeed, sf::Texture* tex1, sf::Texture* tex2, int id, int price, int UPrice, int IncUPrice) : Tower(Pos, hp, IncHp, cooldown, IncCooldown, dmg, IncDmg, radius, IncRadius, bulletoffset, bulletspeed, id, price, UPrice, IncUPrice), base(*tex1), top(*tex2)
{
	float scale = 50.f / float(tex2->getSize().x);
	top.setScale(sf::Vector2f(scale, scale));
	base.setScale(sf::Vector2f(scale, scale));
	base.setColor(sf::Color::Black);

	base.setPosition(Pos);
	top.setPosition(Pos);
}

void Goldmine::Upgrade()
{
	maxhp = maxhp + IncreaseHp;
	hp = maxhp;

	base_cooldown = base_cooldown + IncreaseCooldown;

	dmg = dmg + IncreaseDmg;

	radius = radius + IncreaseRadius;

	switch (currentLevel)
	{
	case 1:
		currentLevel++;
		base.setColor(sf::Color(0.2,  0.2, 0.1));
		break;

	case 2:
		currentLevel++;
		base.setColor(sf::Color(0.4, 0.4, 0.2));
		break;

	case 3:
		currentLevel++;
		base.setColor(sf::Color(0.6, 0.6, 0.3));
		break;

	case 4:
		currentLevel++;
		base.setColor(sf::Color(0.8, 0.8, 0.4));
		break;
	}
}

int Goldmine::getUpgradePrice()
{
	return UpgradePrice + ((currentLevel - 1) * IncreaseUpgradePrice);;
}

int Goldmine::getSellPrice()
{
	return price / 2 + (currentLevel * IncreaseUpgradePrice) / 2;;
}

int Goldmine::getRepairPrice()
{
	return ((maxhp-hp)*5);
}

void Goldmine::GenerateGold(int* Money)
{
	if (cooldown <= 0) 
	{
		*Money += int(dmg);
		cooldown = base_cooldown;
	}
}

void Goldmine::UpdateTower(sf::Time time)
{
	if (cooldown >= 0)
	{
		cooldown = cooldown - time.asSeconds();
	}
}

sf::Vector2f Goldmine::getPosition()
{
	return Position;
}

void Goldmine::draw(sf::RenderWindow& window)
{
	window.draw(top);
	window.draw(base);
}
