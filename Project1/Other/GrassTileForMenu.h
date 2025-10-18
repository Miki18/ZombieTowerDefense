#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

struct GrassTileMenu
{
	sf::Vector2i Position; //Here we store position as number of place (0 - 31 and 0 - 18)
	bool IsDarker = false;
};