#include "Monster.h"

Monster::Monster(sf::Texture& tex, float hp, float speed, int price, int TileSize, int& P_HP, int& P_Money, std::vector<sf::Vector2i>& starts, std::vector<PathPoints>& path, int Monster_ID, sf::Vector2f texSize): PlayerHP(P_HP), PlayerMoney(P_Money)
{
	startpoints = &starts;
	paths = &path;

	health = hp;
	maxhealth = hp;
	this->speed = speed;
	this->TileSize = TileSize;
	texture = tex;
	ID = Monster_ID;
	this->price = price;

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
	body.setTextureRect(sf::IntRect({ 0,0 }, { int(texSize.x), int(texSize.y) }));
	body.setPosition(Position);
	body.setSize(texSize);
	body.setOrigin(sf::Vector2f(texSize.x/2,texSize.y/2));
}

void Monster::DrawMonster(sf::RenderWindow& window)
{
	window.draw(body);
}

void Monster::TakeDamage(float dmg)
{
	health -= dmg;
	if (health <= 0)
	{
		IsDead = true;
		PlayerMoney += price;
	}
}

void Monster::DrawHealth(sf::RenderWindow& window)
{
	if (health / maxhealth < 1)
	{
		sf::RectangleShape back(sf::Vector2f(20, 5));
		back.setFillColor(sf::Color(sf::Color::Black));
		back.setPosition(sf::Vector2f(Position.x - body.getSize().x/2, Position.y - body.getSize().y / 2));
		
		sf::RectangleShape fill(sf::Vector2f(20 * health / maxhealth, 5));
		fill.setFillColor(sf::Color(sf::Color::Red));
		fill.setPosition(sf::Vector2f(Position.x - body.getSize().x / 2, Position.y - body.getSize().y/2));

		window.draw(back);
		window.draw(fill);
	}
}

sf::Vector2f Monster::getPosition()
{
	return Position;
}

void Monster::ChooseDestination()
{
	bool HasDestination = false;

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
			HasDestination = true;
			break;
		}
	}

	if (HasDestination == false)
	{
		IsDead = true;
		PlayerHP--;
	}
}

int Monster::GetID()
{
	return ID;
}
