//State "Menu"
#pragma once
#include "StateManager.h"

class MenuState: public StateManager
{
	public:
		MenuState();

	private:
		enum Screens{
			SelectProfile,
			MainMenu
		};

		nlohmann::json profiles;
		enum Screens current_screen;

		char new_profile_name[15] = "";

		void Input(sf::RenderWindow& window, sf::Clock& DeltaClock);
		void Update(sf::Clock& DeltaClock);
		void Render(sf::RenderWindow& window);

		//Screens in Menu
		void SelectProfileScreen();
		void MainMenuScreen();
};