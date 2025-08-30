#include "MenuState.h"
#include "StateMachine.h"

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

void MenuState::Input(sf::RenderWindow& window, sf::Clock& DeltaClock)
{
	while (auto event = window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(window, *event);
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		else if (const auto* Key = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (Key->button == sf::Mouse::Button::Left)
			{
				StateMachine::Get().ChangeState(new GameState());
			}
		}
	}

	ImGui::SFML::Update(window, DeltaClock.restart());
}

void MenuState::Update(sf::Clock& DeltaClock)
{
	//TODO
}

void MenuState::Render(sf::RenderWindow& window)
{
	window.clear(sf::Color::Blue);

	ImGui::SFML::Render(window);
	window.display();
}
