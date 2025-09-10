#include "CannonTower.h"

CannonTower::CannonTower(sf::Vector2f Pos, sf::Texture* tex1, sf::Texture* tex2, int id)
{
	ID = id;
	Position = Pos;
	tex_base = tex1;
	tex_top = tex2;
}

void CannonTower::draw(sf::RenderWindow& window)
{
	sf::Sprite sprite_base(*tex_base);
	sf::Sprite sprite_top(*tex_top);
	sprite_base.setPosition(Position);
	sprite_top.setPosition(Position);
	sprite_base.setScale(sf::Vector2f(Scale, Scale));
	sprite_top.setScale(sf::Vector2f(Scale, Scale));
	window.draw(sprite_base);
	window.draw(sprite_top);
}