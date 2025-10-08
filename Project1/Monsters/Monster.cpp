#include "Monster.h"

Monster::Monster(sf::Texture& tex, float hp, float speed, int TileSize, int& P_HP, std::vector<sf::Vector2i>& starts, std::vector<sf::Vector2i>& ends, std::vector<PathPoints>& path, int Monster_ID): PlayerHP(P_HP)
{
	startpoints = &starts;
	endpoints = &ends;
	paths = &path;

	health = hp;
	this->speed = speed;
	this->TileSize = TileSize;
	texture = tex;
	ID = Monster_ID;

	int startPos;
	if (starts.size() > 1)
	{
		startPos = rand() % starts.size();
	}
	else
	{
		startPos = 0;
	}
	
	DestinationTile = sf::Vector2i((*startpoints)[startPos].x, (*startpoints)[startPos].y);
	Position = sf::Vector2f(TileSize / 2 + (*startpoints)[startPos].x * TileSize, TileSize / 2 + (*startpoints)[startPos].y * TileSize);
	Destination = Position;
	ChooseDestination();

	body.setTexture(&texture);
	body.setTextureRect(sf::IntRect({ 0,0 }, { 25,50 }));
	body.setPosition(Position);
	body.setSize(sf::Vector2f(25, 50));
	body.setOrigin(sf::Vector2f(13,25));
}

void Monster::DrawMonster(sf::RenderWindow& window)
{
	window.draw(body);
}

void Monster::ChooseDestination()
{
	for (int i = 0; i < endpoints->size(); i++)
	{
		if ((*endpoints)[i] == DestinationTile)
		{
			IsDead = true;
			PlayerHP--;
			return;
		}
	}

	for (int i = 0; i < paths->size(); i++)
	{
		if ((*paths)[i].point == DestinationTile)
		{
			if ((*paths)[i].successors.size() == 1)
			{
				DestinationTile = (*paths)[i].successors[0];
			}
			else
			{
				int Random = rand() % (*paths)[i].successors.size();
				DestinationTile = (*paths)[i].successors[Random];
			}
			Destination = sf::Vector2f(TileSize / 2 + DestinationTile.x * TileSize, TileSize / 2 + DestinationTile.y * TileSize);
			break;
		}
	}
}

int Monster::GetID()
{
	return ID;
}
