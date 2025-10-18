//State "Menu"
#pragma once
#include "StateManager.h"
#include "../Other/GrassTileForMenu.h"

class MenuState : public StateManager
{
public:
	MenuState();

private:
	//Screens with different content
	enum Screens {
		SelectProfile,
		MainMenu,
		SelectLevel,
		Options,
		Settings,
		Credits
	};

	bool changeState = false;

	nlohmann::json profiles;
	enum Screens current_screen;

	char new_profile_name[15] = "";
	bool wrong_nick = false;

	//Map has 1600 x 900 size => 32 x 18 tiles
	const int MapSize[2] = { 32, 18 };   //last on is for UI
	const sf::Vector2f ButtonSize = sf::Vector2f(200, 50);
	const sf::Vector2f NewProfileSize = sf::Vector2f(200, 150);
	const sf::Vector2f LoadProfilesSize = sf::Vector2f(200, 250);

	std::vector<GrassTileMenu> GrassTile;

	void Input(sf::RenderWindow& window, sf::Time time);
	void Update(sf::Time time);
	void Render(sf::RenderWindow& window);

	//Screens in Menu
	void SelectProfileScreen();
	void MainMenuScreen();
	void SelectLevelScreen();
	void OptionsScreen();
	void SettingsScreen();
	void CreditsScreen();

	//UI Elements
	bool LoadProfilesUI();
	void NewProfileUI();
	void ButtonUI(ImVec2 Pos, std::string name, std::function<void()> OnClick);
};