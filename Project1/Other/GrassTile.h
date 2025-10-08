#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

struct GrassTile
{
	sf::Vector2i Position;
	int TowerID = 0;   // 0 means tile is empty
	bool IsDarker = false;
};