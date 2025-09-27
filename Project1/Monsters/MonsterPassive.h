#pragma once
#include "Monster.h"
class MonsterPassive: public Monster
{
	public:
		MonsterPassive(sf::Texture tex, float hp, float speed, int TileSize, std::vector<sf::Vector2i>& starts, std::vector<sf::Vector2i>& ends, std::vector<PathPoints>& paths);
		~MonsterPassive() {};

		void MonsterUpdate(sf::Time time);
	private:
		float UntilNextTex = float(1.f / 7.f);   //because we have 8 frames per second
		int frame = 0;
		void MonsterWalk(sf::Time time);
};

