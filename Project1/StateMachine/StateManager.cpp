#include "StateManager.h"

StateManager::StateManager(): tex_0("Resources/Visuals/UI/UI_Square.png"), tex_1("Resources/Visuals/UI/UI_marked.png")
{
	UI_Sprite.push_back(sf::Sprite(tex_0));
	UI_Sprite.push_back(sf::Sprite(tex_1));
}
