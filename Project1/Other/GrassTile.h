#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

struct GrassTile
{
	sf::Vector2i Position; //Here we store position as number of place (0 - 31 and 0 - 18)
	int TowerID = 0;   // 0 means tile is empty
	bool IsDarker = false;
	bool HasGold = false;   // 0 means it doesn't have gold
	bool HasRuins = false;
};