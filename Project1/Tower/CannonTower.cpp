#include "CannonTower.h"

CannonTower::CannonTower(sf::Vector2f Pos, float hp, float IncHp, float cooldown, float IncCooldown, float dmg, float IncDmg, float radius, float IncRadius, float bulletoffset, float bulletspeed, sf::Texture* tex1, sf::Texture* tex2, int id, int price, int UPrice, int IncUPrice) : Tower(Pos, hp, IncHp, cooldown, IncCooldown, dmg, IncDmg, radius, IncRadius, bulletoffset, bulletspeed, id, price, UPrice, IncUPrice), base(*tex1), top(*tex2)
{
	float scale = 50.f / float(tex2->getSize().x);   //texture is square; x = y
	top.setScale(sf::Vector2f(scale, scale));
	base.setScale(sf::Vector2f(scale, scale));

	Position = sf::Vector2f(Pos.x + 25, Pos.y + 25);
	top.setOrigin(sf::Vector2f(32, 32));

	base.setPosition(Pos);
	top.setPosition(Position);

	Dot.setRadius(2.f);
	Dot.setOrigin(sf::Vector2f(Dot.getRadius(), Dot.getRadius()));
	Dot.setPosition(Position);
	Dot.setFillColor(sf::Color::White);
}

void CannonTower::Upgrade()
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
		break;

	case 2:
		currentLevel++;
		Dot.setFillColor(sf::Color(170, 170, 170));
		break;

	case 3:
		currentLevel++;
		Dot.setFillColor(sf::Color(85, 85, 85));
		break;

	case 4:
		currentLevel++;
		Dot.setFillColor(sf::Color(0, 0, 0));
		break;
	}
}

int CannonTower::getUpgradePrice()
{
	return UpgradePrice + ((currentLevel - 1) * IncreaseUpgradePrice);
}

int CannonTower::getSellPrice()
{
	return price/2 + (currentLevel * IncreaseUpgradePrice)/2;
}

int CannonTower::getRepairPrice()
{
    return int((maxhp-hp)/4);
}

float CannonTower::getBulletSpeed()
{
	return bulletSpeed;
}

bool CannonTower::CanShoot()
{
	if (cooldown <= 0)
	{
		cooldown = base_cooldown;
		return true;
	}
	return false;
}

sf::Vector2f CannonTower::getBulletStartingPosition(sf::Vector2f Dir)
{
	sf::Vector2f StartingPosition = Position - bulletOffset*Dir;
	return StartingPosition;
}

void CannonTower::UpdateTower(sf::Time time)
{
	if (cooldown >= 0)
	{
		cooldown = cooldown - time.asSeconds();
	}
}

sf::Vector2f CannonTower::getPosition()
{
	return Position;
}

sf::Vector2f CannonTower::detectEnemy(sf::Vector2f EnemyPos)
{
	//check if lenght is smaller than radius
	sf::Vector2f tower_enemy = Position - EnemyPos;
	float vec_length = tower_enemy.length();
	if (vec_length <= radius)
	{
		//normalization
		tower_enemy.x = tower_enemy.x / vec_length;
		tower_enemy.y = tower_enemy.y / vec_length;

		float angleRad = std::atan2(-tower_enemy.x, tower_enemy.y);
		float angleDeg = angleRad * 180.f / float(3.14);

		top.setRotation(sf::degrees(angleDeg));

		return tower_enemy;
	}
	return sf::Vector2f(0,0);
}

void CannonTower::draw(sf::RenderWindow& window)
{
	window.draw(base);
	window.draw(top);
	if (currentLevel != 1)
	{
		window.draw(Dot);
	}
}