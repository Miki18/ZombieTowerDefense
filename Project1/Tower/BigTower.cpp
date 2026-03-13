#include "BigTower.h"

BigTower::BigTower(sf::Vector2f Pos, float hp, float IncHp, float cooldown, float IncCooldown, float dmg, float IncDmg, float radius, float IncRadius, float bulletoffset, float bulletspeed, sf::Texture* tex1, sf::Texture* tex2, int id, int price, int UPrice, int IncUPrice, sf::SoundBuffer& tower_shoot) : Tower(Pos, hp, IncHp, cooldown, IncCooldown, dmg, IncDmg, radius, IncRadius, bulletoffset, bulletspeed, id, price, UPrice, IncUPrice, tower_shoot), base(*tex1), top(*tex2)
{
	float scale = 50.f / float(tex2->getSize().x);   //texture is square; x = y
	top.setScale(sf::Vector2f(scale, scale));
	base.setScale(sf::Vector2f(scale, scale));

	Position = sf::Vector2f(Pos.x + 25, Pos.y + 25);
	top.setOrigin(sf::Vector2f(32, 32));

	base.setPosition(Pos);
	top.setPosition(Position);

	DotSmall.setRadius(2.f);
	DotSmall.setOrigin(sf::Vector2f(DotSmall.getRadius(), DotSmall.getRadius()));
	DotSmall.setPosition(Position);
	DotSmall.setFillColor(sf::Color::Black);

	DotMedium.setRadius(8.f);
	DotMedium.setOutlineThickness(2.0f);
	DotMedium.setOutlineColor(sf::Color::Black);
	DotMedium.setOrigin(sf::Vector2f(DotMedium.getRadius(), DotMedium.getRadius()));
	DotMedium.setPosition(Position);
	DotMedium.setFillColor(sf::Color(0, 0, 0, 0));

	DotBig.setRadius(14.f);
	DotBig.setOutlineThickness(2.0f);
	DotBig.setOutlineColor(sf::Color::Black);
	DotBig.setOrigin(sf::Vector2f(DotBig.getRadius(), DotBig.getRadius()));
	DotBig.setPosition(Position);
	DotBig.setFillColor(sf::Color(0, 0, 0, 0));
}

void BigTower::Upgrade()
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
		break;

	case 3:
		currentLevel++;
		break;

	case 4:
		currentLevel++;
		break;
	}
}

int BigTower::getUpgradePrice()
{
	return UpgradePrice + ((currentLevel - 1) * IncreaseUpgradePrice);
}

int BigTower::getSellPrice()
{
	return price / 3 + ((currentLevel - 1) * IncreaseUpgradePrice) / 4;
}

int BigTower::getRepairPrice()
{
	return int(maxhp - hp);
}

float BigTower::getBulletSpeed()
{
	return bulletSpeed;
}

bool BigTower::CanShoot()
{
	if (cooldown <= 0)
	{
		cooldown = base_cooldown;
		return true;
	}
	return false;
}

bool BigTower::HasPermanentBullet()
{
	return true;
}

float BigTower::BulletRadius()
{
	return 10.0f;
}

sf::Vector2f BigTower::getBulletStartingPosition(sf::Vector2f Dir)
{
	sf::Vector2f StartingPosition = Position - bulletOffset * Dir;
	return StartingPosition;
}

void BigTower::UpdateTower(sf::Time time)
{
	if (cooldown >= 0)
	{
		cooldown = cooldown - time.asSeconds();
	}
}

sf::Vector2f BigTower::getPosition()
{
	return Position;
}

sf::Vector2f BigTower::detectEnemy(sf::Vector2f EnemyPos)
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
	return sf::Vector2f(0, 0);
}

void BigTower::draw(sf::RenderWindow& window)
{
	window.draw(base);
	window.draw(top);
	if (currentLevel == 2)
	{
		window.draw(DotSmall);
	}
	else if (currentLevel == 3)
	{
		window.draw(DotBig);
	}
	else if (currentLevel == 4)
	{
		window.draw(DotBig);
		window.draw(DotMedium);
	}
	else if (currentLevel == 5)
	{
		window.draw(DotBig);
		window.draw(DotMedium);
		window.draw(DotSmall);
	}
}