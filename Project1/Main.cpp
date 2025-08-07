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

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Window");

	if (!ImGui::SFML::Init(window))
		return -1;

	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	sf::Clock deltaClock;

	while (window.isOpen())
	{
		while (auto event = window.pollEvent())
		{
			ImGui::SFML::ProcessEvent(window, *event);
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Hello from ImGui!");
		ImGui::Text("This is working!");
		ImGui::End();

		window.clear();

		window.draw(shape);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}
