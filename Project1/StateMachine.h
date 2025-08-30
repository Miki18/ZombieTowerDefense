#pragma once
#include "StateManager.h"
#include "MenuState.h"
#include "GameState.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class StateMachine
{
	private:
		StateManager* State = new MenuState();

		StateMachine() {};
		static StateMachine Instance;

	protected:
		sf::Clock deltaClock;

	public:
		static StateMachine& Get();
		void ChangeState(StateManager* newState);
		void Run(sf::RenderWindow& window);
};

