#include "Ultimate.h"

Ultimate::Ultimate(sf::Vector2f Pos, float hp, float IncHp, float cooldown, float IncCooldown, float dmg, float IncDmg, float radius, float IncRadius, float bulletoffset, float bulletspeed, sf::Texture* tex1, sf::Texture* tex2, int id, int price, int UPrice, int IncUPrice, sf::SoundBuffer& tower_shoot) : Tower(Pos, hp, IncHp, cooldown, IncCooldown, dmg, IncDmg, radius, IncRadius, bulletoffset, bulletspeed, id, price, UPrice, IncUPrice, tower_shoot), base(*tex1), top(*tex2)
{
	float scale = 50.f / float(tex2->getSize().x);   //texture is square; x = y
	top.setScale(sf::Vector2f(scale, scale));
	base.setScale(sf::Vector2f(scale, scale));

	Position = sf::Vector2f(Pos.x + 25, Pos.y + 25);
	top.setOrigin(sf::Vector2f(32, 32));

	base.setPosition(Pos);
	top.setPosition(Position);

	Dot1.setRadius(2.f);
	Dot1.setOrigin(sf::Vector2f(Dot1.getRadius() + 10, Dot1.getRadius() - 12));
	Dot1.setPosition(Position);
	Dot1.setFillColor(sf::Color::White);

	Dot2.setRadius(2.f);
	Dot2.setOrigin(sf::Vector2f(Dot2.getRadius() + 10, Dot2.getRadius() - 1));
	Dot2.setPosition(Position);
	Dot2.setFillColor(sf::Color::White);

	Dot3.setRadius(2.f);
	Dot3.setOrigin(sf::Vector2f(Dot3.getRadius() -10, Dot3.getRadius() -12));
	Dot3.setPosition(Position);
	Dot3.setFillColor(sf::Color::White);

	Dot4.setRadius(2.f);
	Dot4.setOrigin(sf::Vector2f(Dot4.getRadius() -10, Dot4.getRadius() - 1));
	Dot4.setPosition(Position);
	Dot4.setFillColor(sf::Color::White);

	Strip.setOutlineThickness(0);
	Strip.setFillColor(sf::Color(100, 100, 100, 255));
	Strip.setSize(sf::Vector2f(4, 17));
	Strip.setOrigin(sf::Vector2f(2, 23));
	Strip.setPosition(Position);
}

void Ultimate::Upgrade()
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
		Dot1.setFillColor(sf::Color::Black);
		Dot3.setFillColor(sf::Color::Black);
		break;

	case 4:
		currentLevel++;
		Strip.setFillColor(sf::Color::Black);
		Dot2.setFillColor(sf::Color::Black);
		Dot4.setFillColor(sf::Color::Black);
		break;
	}
}

int Ultimate::getUpgradePrice()
{
	return UpgradePrice + ((currentLevel - 1) * IncreaseUpgradePrice);
}

int Ultimate::getSellPrice()
{
	return price / 3 + ((currentLevel - 1) * IncreaseUpgradePrice) / 3;
}

int Ultimate::getRepairPrice()
{
	return int(maxhp - hp);
}

float Ultimate::getBulletSpeed()
{
	return bulletSpeed;
}

bool Ultimate::CanShoot()
{
	if (cooldown <= 0)
	{
		cooldown = base_cooldown;
		return true;
	}
	return false;
}

sf::Vector2f Ultimate::getBulletStartingPosition(sf::Vector2f Dir)
{
	sf::Vector2f StartingPosition = Position - bulletOffset * Dir;
	return StartingPosition;
}

void Ultimate::UpdateTower(sf::Time time)
{
	if (cooldown >= 0)
	{
		cooldown = cooldown - time.asSeconds();
	}
}

sf::Vector2f Ultimate::getPosition()
{
	return Position;
}

sf::Vector2f Ultimate::detectEnemy(sf::Vector2f EnemyPos)
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
		Dot1.setRotation(sf::degrees(angleDeg));
		Dot2.setRotation(sf::degrees(angleDeg));
		Dot3.setRotation(sf::degrees(angleDeg));
		Dot4.setRotation(sf::degrees(angleDeg));
		Strip.setRotation(sf::degrees(angleDeg));

		return tower_enemy;
	}
	return sf::Vector2f(0, 0);
}

void Ultimate::draw(sf::RenderWindow& window)
{
	window.draw(base);
	window.draw(top);

	if (currentLevel > 1)
	{
		window.draw(Dot1);
		window.draw(Dot3);
		if (currentLevel > 2)
		{
			window.draw(Strip);
			window.draw(Dot2);
			window.draw(Dot4);
		}
	}
}