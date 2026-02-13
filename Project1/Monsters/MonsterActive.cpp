#include "MonsterActive.h"

MonsterActive::MonsterActive(sf::Texture tex, float hp, float speed, int price, int TileSize, int& P_HP, int& P_Money, std::vector<sf::Vector2i>& starts, std::vector<PathPoints>& paths, int MonsterID, sf::Vector2f texSize, sf::Texture tex2, float Range, sf::Vector2f BulletPos, int Damage) : Monster(tex, hp, speed, price, TileSize, P_HP, P_Money, starts, paths, MonsterID, texSize)
{
	this->Attack_texture = tex2;
	this->range = Range;
	this->Bulletspawn = BulletPos;
	this->Dmg = Damage;
}

bool MonsterActive::IsShooter()
{
	return true;
}

void MonsterActive::ResetCooldown(float cooldown)
{
	this->Cooldown = cooldown;
}

float MonsterActive::GetRange()
{
	return range;
}

float MonsterActive::GetCooldown()
{
    return Cooldown;
}

int MonsterActive::GetDmg()
{
	return Dmg;
}

sf::Vector2f MonsterActive::getBulletStartPosition()
{
	return sf::Vector2f(getPosition() - Bulletspawn);
}

void MonsterActive::MonsterUpdate(sf::Time time)
{
	MonsterWalk(time);
}

void MonsterActive::LowerCooldown(sf::Time time)
{
	Cooldown = Cooldown - time.asSeconds();
}

void MonsterActive::ChangeAnimation(bool A)
{
	if (A == true)
	{
		if (MonsterState == 1)
		{
			MonsterState = 0;
			frame = 0;
			body.setTexture(&Attack_texture);
		}
	}
	else
	{
		if (MonsterState == 0)
		{
			MonsterState = 1;
			frame = 0;
			body.setTexture(&texture);
		}
	}
}

void MonsterActive::PlayAnimation(sf::Time time)
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
		body.setTextureRect(sf::IntRect({ 100 * frame,0 }, { 100,100 }));   //WARNING
		UntilNextTex = float(1.f / 8.f);
	}
}

void MonsterActive::MonsterWalk(sf::Time time)
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
