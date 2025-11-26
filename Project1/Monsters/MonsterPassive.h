#pragma once
#include "Monster.h"
class MonsterPassive: public Monster
{
	public:
		MonsterPassive(sf::Texture tex, float hp, float speed, int price, int TileSize, int& P_HP, int& P_Money, std::vector<sf::Vector2i>& starts, std::vector<PathPoints>& paths, int MonsterID);
		~MonsterPassive() {};

		void MonsterUpdate(sf::Time time);
	private:
		float UntilNextTex = float(1.f / 7.f);   //because we have 7 frames per second
		int frame = 0;
		void MonsterWalk(sf::Time time);
};

