#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <string>
#include <vector>
#include <fstream>
#include <cmath>

#include "StateMachine/StateMachine.h"
#include "StateMachine/StateManager.h"

int main()
{
	//create render window
	sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Game", sf::State::Fullscreen);

	//Init ImGui and SFML
	ImGui::SFML::Init(window);

	//Antialliasing
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 8;

	//create state machine and set state to MenuState
	StateMachine& machine = StateMachine::Get();
	machine.ChangeState(new MenuState());

	//Run game loop
	machine.Run(window);

	//Shut down after program goes out of game loop
	ImGui::SFML::Shutdown();
}
