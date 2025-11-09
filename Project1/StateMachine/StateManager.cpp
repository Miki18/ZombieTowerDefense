#include "StateManager.h"

StateManager::StateManager()
{
	UI_HeartTex.loadFromFile("Resources/Visuals/UI/Heart.png");
	UI_MoneyTex.loadFromFile("Resources/Visuals/UI/Money.png");
	UI_ShovelTex.loadFromFile("Resources/Visuals/UI/Shovel.png");
	UI_DolarTex.loadFromFile("Resources/Visuals/UI/UI_DolarButton.png");
	UI_RepairTex.loadFromFile("Resources/Visuals/UI/UI_RepairButton.png");
	UI_UpgradeTex.loadFromFile("Resources/Visuals/UI/UI_UpgradeButton.png");
	UI_TitleTex.loadFromFile("Resources/Visuals/UI/Title.png");

	UI_Sprite.push_back(sf::Sprite(UI_HeartTex));
	UI_Sprite.push_back(sf::Sprite(UI_MoneyTex));
	UI_Sprite.push_back(sf::Sprite(UI_ShovelTex));
	UI_Sprite.push_back(sf::Sprite(UI_DolarTex));
	UI_Sprite.push_back(sf::Sprite(UI_RepairTex));
	UI_Sprite.push_back(sf::Sprite(UI_UpgradeTex));
	UI_Sprite.push_back(sf::Sprite(UI_TitleTex));

	UI_Sprite[SpriteList(UI_Shovel)].setScale(sf::Vector2f(TilesSize / UI_Sprite[SpriteList(UI_Shovel)].getTexture().getSize().x, TilesSize / UI_Sprite[SpriteList(UI_Shovel)].getTexture().getSize().y));
}
