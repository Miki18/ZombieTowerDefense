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
		bool Levels[9] = {};
		float SoundVolume = 100.0f;
		float MusicVolume = 100.0f;
		int PlayerStat[7] = {};

		StateMachine() {};
		static StateMachine Instance;

	protected:
		sf::Clock deltaClock;

	public:
		static StateMachine& Get();
		void ChangeState(StateManager* newState);
		void Run(sf::RenderWindow& window);

		int LevelNumber = 6;
		int SelectedLevel = 0;
		std::string getSelectedProfile();
		void setSelectedProfile(std::string nick);
		void PassLevel(int index, bool Check);
		void PassPlayerStat(int id, int number);
		int GetPlayerStat(int id);
		bool GetLevelStatus(int index);
		bool VolumeSetting(ImVec2 Pos, std::string name, bool IsMusic);
		float GetVolume(bool isMusic);
		bool IsFullscreen = true;
};

