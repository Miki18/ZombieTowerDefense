#include "MortarTower.h"

MortarTower::MortarTower(sf::Vector2f Pos, float hp, float IncHp, float cooldown, float IncCooldown, float dmg, float IncDmg, float radius, float IncRadius, float bulletoffset, float bulletspeed, sf::Texture* tex1, sf::Texture* tex2, int id, int price, int UPrice, int IncUPrice, sf::SoundBuffer& tower_shoot) : Tower(Pos, hp, IncHp, cooldown, IncCooldown, dmg, IncDmg, radius, IncRadius, bulletoffset, bulletspeed, id, price, UPrice, IncUPrice, tower_shoot), base(*tex1), top(*tex2)
{
	float scale = 50.f / float(tex2->getSize().x);   //texture is square; x = y
	top.setScale(sf::Vector2f(scale, scale));
	base.setScale(sf::Vector2f(scale, scale));

	Position = sf::Vector2f(Pos.x + 25, Pos.y + 25);
	top.setOrigin(sf::Vector2f(32, 32));

	base.setPosition(Pos);
	top.setPosition(Position);

	first.setSize(sf::Vector2f(16, 2));
	first.setOrigin(sf::Vector2f(8, 20));
	first.setPosition(Position);
	first.setFillColor(sf::Color::Black);

	second.setSize(sf::Vector2f(18, 2));
	second.setOrigin(sf::Vector2f(9, 15));
	second.setPosition(Position);
	second.setFillColor(sf::Color::Black);

	third.setSize(sf::Vector2f(20, 2));
	third.setOrigin(sf::Vector2f(10, 10));
	third.setPosition(Position);
	third.setFillColor(sf::Color::Black);

	fourth.setSize(sf::Vector2f(22, 2));
	fourth.setOrigin(sf::Vector2f(11, 5));
	fourth.setPosition(Position);
	fourth.setFillColor(sf::Color::Black);
}

void MortarTower::Upgrade()
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

int MortarTower::getUpgradePrice()
{
	return UpgradePrice + ((currentLevel - 1) * IncreaseUpgradePrice);
}

int MortarTower::getSellPrice()
{
	return price / 3 + ((currentLevel - 1) * IncreaseUpgradePrice) / 3;
}

int MortarTower::getRepairPrice()
{
	return int((maxhp - hp) / 2.0);
}

float MortarTower::getBulletSpeed()
{
	return bulletSpeed;
}

bool MortarTower::CanShoot()
{
	if (cooldown <= 0)
	{
		cooldown = base_cooldown;
		return true;
	}
	return false;
}

sf::Vector2f MortarTower::getBulletStartingPosition(sf::Vector2f Dir)
{
	sf::Vector2f StartingPosition = Position - bulletOffset * Dir;
	return StartingPosition;
}

void MortarTower::UpdateTower(sf::Time time)
{
	if (cooldown >= 0)
	{
		cooldown = cooldown - time.asSeconds();
	}
}

bool MortarTower::HasExplosiveBullet()
{
	return true;
}

sf::Vector2f MortarTower::getPosition()
{
	return Position;
}

sf::Vector2f MortarTower::detectEnemy(sf::Vector2f EnemyPos)
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
		first.setRotation(sf::degrees(angleDeg));
		second.setRotation(sf::degrees(angleDeg));
		third.setRotation(sf::degrees(angleDeg));
		fourth.setRotation(sf::degrees(angleDeg));

		return tower_enemy;
	}
	return sf::Vector2f(0, 0);
}

void MortarTower::draw(sf::RenderWindow& window)
{
	window.draw(base);
	window.draw(top);
	if (currentLevel >= 2)
	{
		window.draw(first);

		if (currentLevel >= 3)
		{
			window.draw(second);

			if (currentLevel >= 4)
			{
				window.draw(third);

				if (currentLevel == 5)
				{
					window.draw(fourth);
				}
			}
		}
	}
}