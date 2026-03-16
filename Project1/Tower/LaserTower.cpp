#include "LaserTower.h"

LaserTower::LaserTower(sf::Vector2f Pos, float hp, float IncHp, float cooldown, float IncCooldown, float dmg, float IncDmg, float radius, float IncRadius, float bulletoffset, float bulletspeed, sf::Texture* tex1, sf::Texture* tex2, int id, int price, int UPrice, int IncUPrice, sf::SoundBuffer& tower_shoot) : Tower(Pos, hp, IncHp, cooldown, IncCooldown, dmg, IncDmg, radius, IncRadius, bulletoffset, bulletspeed, id, price, UPrice, IncUPrice, tower_shoot), base(*tex1), top(*tex2)
{
	float scale = 50.f / float(tex2->getSize().x);   //texture is square; x = y
	top.setScale(sf::Vector2f(scale, scale));
	base.setScale(sf::Vector2f(scale, scale));

	Position = sf::Vector2f(Pos.x + 25, Pos.y + 25);
	top.setOrigin(sf::Vector2f(32, 32));

	base.setPosition(Pos);
	top.setPosition(Position);

	Mark.setFillColor(sf::Color::White);
	Mark.setSize(sf::Vector2f(2, 11));
	Mark.setOutlineColor(sf::Color::Black);
	Mark.setOutlineThickness(1);
	Mark.setOrigin(sf::Vector2f(Mark.getGeometricCenter().x, Mark.getGeometricCenter().y - 1));
	Mark.setPosition(Position);
}

void LaserTower::Upgrade()
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
		Mark.setFillColor(sf::Color(255, 170, 170, 255));
		break;
	case 3:
		currentLevel++;
		Mark.setFillColor(sf::Color(255, 85, 85, 255));
		break;
	case 4:
		currentLevel++;
		Mark.setFillColor(sf::Color(255, 0, 0, 255));
		break;
	}
}

int LaserTower::getUpgradePrice()
{
	return UpgradePrice + ((currentLevel - 1) * IncreaseUpgradePrice);
}

int LaserTower::getSellPrice()
{
	return price / 5 + ((currentLevel - 1) * IncreaseUpgradePrice) / 2;
}

int LaserTower::getRepairPrice()
{
	return int((maxhp - hp) / 2);
}

float LaserTower::getBulletSpeed()
{
	return bulletSpeed;
}

bool LaserTower::CanShoot()
{
	if (cooldown <= 0)
	{
		cooldown = base_cooldown;
		return true;
	}
	return false;
}

sf::Vector2f LaserTower::getBulletStartingPosition(sf::Vector2f Dir)
{
	sf::Vector2f StartingPosition = Position - bulletOffset * Dir;
	return StartingPosition;
}

void LaserTower::UpdateTower(sf::Time time)
{
	if (cooldown >= 0)
	{
		cooldown = cooldown - time.asSeconds();
	}
}

sf::Vector2f LaserTower::getPosition()
{
	return Position;
}

sf::Vector2f LaserTower::detectEnemy(sf::Vector2f EnemyPos)
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
		Mark.setRotation(sf::degrees(angleDeg));

		return tower_enemy;
	}
	return sf::Vector2f(0, 0);
}

void LaserTower::draw(sf::RenderWindow& window)
{

	window.draw(base);
	window.draw(top);
	if (currentLevel != 1)
	{
		window.draw(Mark);
	}
}