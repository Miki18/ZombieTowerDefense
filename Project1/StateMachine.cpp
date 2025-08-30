#include "StateMachine.h"

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

StateMachine& StateMachine::Get()
{
	static StateMachine Instance;
	return Instance;
}

void StateMachine::ChangeState(StateManager* newState)
{
	delete State;
	State = newState;
}

void StateMachine::Run(sf::RenderWindow& window)
{
	deltaClock.start();
	while (window.isOpen())
	{
		State->Input(window, deltaClock);
		State->Update(deltaClock);
		State->Render(window);
	}
}
