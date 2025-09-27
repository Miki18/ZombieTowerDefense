#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

struct PathPoints
{
	sf::Vector2i point;
	std::vector<sf::Vector2i> successors;
};