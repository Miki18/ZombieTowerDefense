#include "MonsterPassive.h"

MonsterPassive::MonsterPassive(sf::Texture tex, float hp, float speed, int TileSize, std::vector<sf::Vector2i>& starts, std::vector<sf::Vector2i>& ends, std::vector<PathPoints>& paths) : Monster(tex, hp, speed, TileSize,starts, ends, paths)
{

}

void MonsterPassive::MonsterUpdate(sf::Time time)
{
	MonsterWalk(time);
}

void MonsterPassive::MonsterWalk(sf::Time time)
{
	if (Position.x == Destination.x)
	{
		if (Position.y > Destination.y)
		{
			Position.y = Position.y - speed * time.asSeconds();
			if (Position.y < Destination.y)
			{
				Position.y = Destination.y;
				ChooseDestination();
			}
		}
		else
		{
			Position.y = Position.y + speed * time.asSeconds();
			if (Position.y > Destination.y)
			{
				Position.y = Destination.y;
				ChooseDestination();
			}
		}
	}
	else
	{
		if (Position.x > Destination.x)
		{
			Position.x = Position.x - speed * time.asSeconds();
			if (Position.x < Destination.x)
			{
				Position.x = Destination.x;
				ChooseDestination();
			}
		}
		else
		{
			Position.x = Position.x + speed * time.asSeconds();
			if (Position.x > Destination.x)
			{
				Position.x = Destination.x;
				ChooseDestination();
			}
		}
	}

	//Move dealing with texture to other function
	UntilNextTex = UntilNextTex - time.asSeconds();
	frame++;
	if (frame > 7)
	{
		frame = 0;
	}
	if (UntilNextTex < 0)
	{
		body.setTexture(&texture);
		body.setTextureRect(sf::IntRect({ 25*frame,0 }, { 25,50 }));
		UntilNextTex = float(1.f/7.f);
	}
	body.setPosition(Position);
}
