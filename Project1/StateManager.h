#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class StateMachine;

class StateManager
{
	protected:
		StateMachine *statemachine;

	public:
		virtual void Input(sf::RenderWindow& window, sf::Clock& DeltaClock) {};
		virtual void Update(sf::Clock& DeltaClock) {};
		virtual void Render(sf::RenderWindow& window) {};
};

