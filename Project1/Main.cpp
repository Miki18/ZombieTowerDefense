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

#include "StateMachine.h"
#include "StateManager.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Window");

	if (!ImGui::SFML::Init(window))
		return -1;

	StateMachine& machine = StateMachine::Get();

	machine.Run(window);

	ImGui::SFML::Shutdown();
}
