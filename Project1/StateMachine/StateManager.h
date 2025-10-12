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
	private:
		//UI
		sf::Texture UI_Square_Tex;
		sf::Texture UI_Marked_Tex;
		sf::Texture UI_HeartTex;
		sf::Texture UI_MoneyTex;
		sf::Texture UI_BigRectangleTex;

	protected:
		const float ScreenSize[2] = {1600, 900};
		const int TilesSize = 50;

		//Sprite vector
		enum SpriteList
		{
			UI_Square,
			UI_Marked,
			UI_Heart,
			UI_Money,
			UI_BigRectangle
		};

		std::vector<sf::Sprite> UI_Sprite;

	public:
		StateManager();
		virtual void Input(sf::RenderWindow& window, sf::Time time) {};
		virtual void Update(sf::Time time) {};
		virtual void Render(sf::RenderWindow& window) {};
};

