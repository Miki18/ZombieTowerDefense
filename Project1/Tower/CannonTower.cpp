#include "CannonTower.h"

CannonTower::CannonTower(sf::Vector2f Pos, sf::Texture* tex1, sf::Texture* tex2, int id) : base(*tex1), top(*tex2)
{
	ID = id;
	Position = Pos;

	base.setPosition(Pos);
	top.setPosition(Pos);
}

void CannonTower::draw(sf::RenderWindow& window)
{
	window.draw(base);
	window.draw(top);
}