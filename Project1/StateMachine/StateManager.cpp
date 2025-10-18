#include "StateManager.h"

StateManager::StateManager()
{
	UI_Square_Tex.loadFromFile("Resources/Visuals/UI/UI_Square.png");
	UI_Marked_Tex.loadFromFile("Resources/Visuals/UI/UI_marked.png");
	UI_HeartTex.loadFromFile("Resources/Visuals/UI/Heart.png");
	UI_MoneyTex.loadFromFile("Resources/Visuals/UI/Money.png");
	UI_BigRectangleTex.loadFromFile("Resources/Visuals/UI/PauseUI.png");
	UI_RectangleButtonTex.loadFromFile("Resources/Visuals/UI/UI_MenuButton.png");
	UI_NewProfileTex.loadFromFile("Resources/Visuals/UI/NewProfileUI.png");
	UI_LoadProfilesTex.loadFromFile("Resources/Visuals/UI/LoadProfilesUI.png");

	UI_Sprite.push_back(sf::Sprite(UI_Square_Tex));
	UI_Sprite.push_back(sf::Sprite(UI_Marked_Tex));
	UI_Sprite.push_back(sf::Sprite(UI_HeartTex));
	UI_Sprite.push_back(sf::Sprite(UI_MoneyTex));
	UI_Sprite.push_back(sf::Sprite(UI_BigRectangleTex));
	UI_Sprite.push_back(sf::Sprite(UI_RectangleButtonTex));
	UI_Sprite.push_back(sf::Sprite(UI_NewProfileTex));
	UI_Sprite.push_back(sf::Sprite(UI_LoadProfilesTex));

	UI_Sprite[2].setScale(sf::Vector2f(TilesSize / UI_Sprite[2].getTexture().getSize().x, TilesSize / UI_Sprite[2].getTexture().getSize().y));
}
