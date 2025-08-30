#pragma once
#include "StateManager.h"

class GameState: public StateManager
{
	private:
		void Input(sf::RenderWindow& window, sf::Clock& DeltaClock);
		void Update(sf::Clock& DeltaClock);
		void Render(sf::RenderWindow& window);
};

