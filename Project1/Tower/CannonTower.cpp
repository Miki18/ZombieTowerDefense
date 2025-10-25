#include "CannonTower.h"

CannonTower::CannonTower(sf::Vector2f Pos, float hp, float cooldown, float dmg, float radius, float bulletoffset, sf::Texture* tex1, sf::Texture* tex2, int id, int price) : Tower(hp, cooldown, dmg, radius, bulletoffset, id, price), base(*tex1), top(*tex2)
{
	float scale = 50.f / float(tex2->getSize().x);   //texture is square; x = y
	top.setScale(sf::Vector2f(scale, scale));
	base.setScale(sf::Vector2f(scale, scale));

	Position = sf::Vector2f(Pos.x + 25, Pos.y + 25);
	top.setOrigin(sf::Vector2f(32, 32));

	base.setPosition(Pos);
	top.setPosition(Position);
}

int CannonTower::getSellPrice()
{
	return price/2;
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
	//std::cout << Dir.x << " "<< Dir.y << std::endl;
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
}