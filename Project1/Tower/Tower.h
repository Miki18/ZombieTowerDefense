//Class that conntects all tower classes
#pragma once
#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <iostream>

class Tower
{
	protected:
		const float TileSize = 50.0f;

	public:
		Tower() {};
		~Tower() {};

		virtual void draw(sf::RenderWindow& window);
};

