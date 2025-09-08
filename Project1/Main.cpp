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
	sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Game", sf::State::Fullscreen);

	if (!ImGui::SFML::Init(window))
		return -1;

	StateMachine& machine = StateMachine::Get();
	machine.ChangeState(new MenuState());

	machine.Run(window);

	ImGui::SFML::Shutdown();
}
