#include "CannonTower.h"

CannonTower::CannonTower(sf::Vector2f Pos, float hp, float cooldown, float dmg, float radius, sf::Vector2f bulletpoints, sf::Texture* tex1, sf::Texture* tex2, int id) : Tower(hp, cooldown, dmg, radius, bulletpoints, id), base(*tex1), top(*tex2)
{
	Position = Pos;

	base.setPosition(Pos);
	top.setPosition(Pos);
	float scale = 50.f/float(tex2->getSize().x);   //texture is square; x = y
	top.setScale(sf::Vector2f(scale , scale));
	base.setScale(sf::Vector2f(scale, scale));
}

void CannonTower::draw(sf::RenderWindow& window)
{
	window.draw(base);
	window.draw(top);
}