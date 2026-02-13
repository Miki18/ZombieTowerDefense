#include "MonsterPassive.h"

MonsterPassive::MonsterPassive(sf::Texture tex, float hp, float speed, int price, int TileSize, int& P_HP, int& P_Money, std::vector<sf::Vector2i>& starts, std::vector<PathPoints>& paths, int MonsterID, sf::Vector2f texSize) : Monster(tex, hp, speed, price, TileSize, P_HP, P_Money, starts, paths, MonsterID, texSize)
{

}

bool MonsterPassive::IsShooter()
{
	return false;
}

void MonsterPassive::MonsterUpdate(sf::Time time)
{
	MonsterWalk(time);
}

void MonsterPassive::PlayAnimation(sf::Time time)
{
	UntilNextTex = UntilNextTex - time.asSeconds();
	if (UntilNextTex < 0)
	{
		frame++;
		if (frame > 7)
		{
			frame = 0;
		}
		body.setTexture(&texture);
		body.setTextureRect(sf::IntRect({ 50 * frame,0 }, { 50,100 }));   //WARNING
		UntilNextTex = float(1.f / 7.f);
	}
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

	body.setPosition(Position);
}
