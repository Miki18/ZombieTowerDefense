//Class that have game loop and change states.

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
		StateManager* State = nullptr;
		std::string selected_profile = "";

		StateMachine() {};
		static StateMachine Instance;

	protected:
		sf::Clock deltaClock;

	public:
		static StateMachine& Get();
		void ChangeState(StateManager* newState);
		void Run(sf::RenderWindow& window);

		std::string getSelectedProfile();
		void setSelectedProfile(std::string nick);
};

