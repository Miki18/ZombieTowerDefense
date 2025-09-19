#include "StateManager.h"

StateManager::StateManager()
{
	UI_Square_Tex.loadFromFile("Resources/Visuals/UI/UI_Square.png");
	UI_Marked_Tex.loadFromFile("Resources/Visuals/UI/UI_marked.png");

	UI_Sprite.push_back(sf::Sprite(UI_Square_Tex));
	UI_Sprite.push_back(sf::Sprite(UI_Marked_Tex));
}
