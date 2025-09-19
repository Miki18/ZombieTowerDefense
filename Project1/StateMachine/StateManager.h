//Class that connects all states. Every state has this class. This is an Abstract Class

#pragma once
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <algorithm>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class StateMachine;

class StateManager
{
	protected:
		const float ScreenSize[2] = {1600, 900};

		//UI
		sf::Texture UI_Square_Tex;
		sf::Texture UI_Marked_Tex;
		std::vector<sf::Sprite> UI_Sprite;

	public:
		StateManager();
		virtual void Input(sf::RenderWindow& window, sf::Clock& DeltaClock) {};
		virtual void Update(sf::Clock& DeltaClock) {};
		virtual void Render(sf::RenderWindow& window) {};
};

