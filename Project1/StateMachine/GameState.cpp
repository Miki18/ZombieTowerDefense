#include "GameState.h"
#include "StateMachine.h"
#include "LoadLevel.inl"

GameState::GameState()
{
	GenerateGrassTiles();

	LoadLevelData();

	LoadRoadTiles();

	LoadObjects();

	LoadPaths();

	LoadMonsters();

	LoadTowerTextures();

	LoadSettings();
}

void GameState::Input(sf::RenderWindow& window, sf::Time time)
{
	while (auto event = window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(window, *event);
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		else
		{
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::P)
				{
					IsGamePaused = !IsGamePaused;
				}
				/*if (keyPressed->scancode == sf::Keyboard::Scancode::Num1)
				{
					SelectedTower = 0;
				}*/
				/*else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2)
				{
					SelectedTower = 1;
				}*/
			}
		}
	}

	ImGui::SFML::Update(window, time);
}

void GameState::Update(sf::Time time)
{	
	//Grass tiles
	for (int i = 0; i < grass_tile.size(); i++)
	{
		ImGui::SetNextWindowSize(ImVec2(TilesSize, TilesSize));
		ImGui::SetNextWindowPos(ImVec2(grass_tile[i].Position.x * TilesSize, grass_tile[i].Position.y * TilesSize));
		std::string name = std::to_string(i);   //names need to be unique

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoScrollbar);
		name = name + "button";

		if (grass_tile[i].IsDarker)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.25f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.60f));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.45f));
		}

		if (ImGui::Button(("##" + name).c_str(), ImVec2(TilesSize, TilesSize)) and IsGamePaused == false)
		{
			if (grass_tile[i].TowerID == 0)
			{
				if (SelectedTower == 0 and Money >= towersvalues[0]->price)
				{
					towers.emplace_back(std::make_unique<CannonTower>(
						sf::Vector2f(grass_tile[i].Position.x * TilesSize, grass_tile[i].Position.y * TilesSize),
						towersvalues[0]->hp, towersvalues[0]->cooldown, towersvalues[0]->dmg, towersvalues[0]->radius, towersvalues[0]->bulletpoint, &towersvalues[0]->base, &towersvalues[0]->top, TowerID));
					grass_tile[i].TowerID = TowerID;
					Money = Money - towersvalues[0]->price;
					TowerID++;
				}
			}
			else if (grass_tile[i].TowerID != 0)
			{
				int T_ID = grass_tile[i].TowerID;
				for (int j = 0; j < towers.size(); j++)
				{
					//DRAW CIRCLE SYSTEM
				}
			}
		}

		ImGui::PopStyleColor(2);
		ImGui::End();
		ImGui::PopStyleVar(1);
	}

	if (IsGamePaused == true)
	{
		ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2, ScreenSize[1] / 2));
		ImGui::Begin("PausedGameText");
		ImGui::Text("Game is Paused; click P to unpause");
		ImGui::End();
		return;
	}

	//Select tower UI
	//SelectTowerUI();

	//Heart and Money UI
	ShowHealtAndMoney();

	//Update Monster
	UpdateMonsters(time);

}

void GameState::Render(sf::RenderWindow& window)
{
	window.clear(sf::Color{60, 160, 0, 255}); //Background color RGBA

	for (int i = 0; i < RoadTiles.size(); i++)
	{
		window.draw(RoadTiles[i]);
	}

	ImGui::SFML::Render(window);

	for (int i = 0; i < towers.size(); i++)
	{
		towers[i]->draw(window);
	}

	for (int i = 0; i < monsters.size(); i++)
	{
		monsters[i]->DrawMonster(window);
	}

	window.display();
}

void GameState::ShowHealtAndMoney()
{
	int correct_UI_location = -10;
	
	//health
	ImGui::SetNextWindowPos(ImVec2(correct_UI_location, ScreenSize[1] - TilesSize + correct_UI_location));
	ImGui::Begin("heart", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	ImGui::Image(sf::Sprite(UI_Sprite[SpriteList(UI_Heart)]), sf::Vector2f(50, 50), sf::Color(255, 255, 255, 255));
	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	ImDrawList* drawlist = ImGui::GetWindowDrawList();
	ImVec2 TextSize = ImGui::CalcTextSize(std::to_string(Health).c_str());
	cursorPos = ImVec2(cursorPos.x + TilesSize / 2 - TextSize.x / 2, cursorPos.y - TilesSize / 2 - TextSize.y);
	drawlist->AddText(cursorPos, IM_COL32_WHITE, std::to_string(Health).c_str());
	ImGui::End();

	int Gap = 10;

	//Money
	ImGui::SetNextWindowPos(ImVec2(correct_UI_location + TilesSize + Gap, ScreenSize[1] - TilesSize + correct_UI_location));
	ImGui::Begin("Money", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	ImGui::Image(sf::Sprite(UI_Sprite[SpriteList(UI_Money)]), sf::Vector2f(100, 50), sf::Color(255, 255, 255, 255));
	cursorPos = ImGui::GetCursorScreenPos();
	drawlist = ImGui::GetWindowDrawList();
	TextSize = ImGui::CalcTextSize(std::to_string(Money).c_str());
	cursorPos = ImVec2(cursorPos.x + TilesSize*2 / 2 - TextSize.x / 2, cursorPos.y - TilesSize/ 2 - TextSize.y);
	drawlist->AddText(cursorPos, IM_COL32_WHITE, std::to_string(Money).c_str());
	ImGui::End();
}

void GameState::SelectTowerUI()
{
	for (int i = 0; i < TowerTypes; i++)
	{
		ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] - TilesSize * TowerTypes + TilesSize * i, ScreenSize[1] - TilesSize));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		std::string name = "UI" + std::to_string(i);

		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

		name = "buttonUI" + std::to_string(i);

		if (i == SelectedTower)
		{
			ImGui::ImageButton(name.c_str(), UI_Sprite[SpriteList(UI_Marked)], sf::Vector2f(50, 50), sf::Color(255, 255, 255, 255));
		}
		else
		{
			if (ImGui::ImageButton(name.c_str(), UI_Sprite[SpriteList(UI_Square)], sf::Vector2f(50, 50), sf::Color(255, 255, 255, 255)))
			{
				SelectedTower = i;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar(1);
	}
}

void GameState::UpdateMonsters(sf::Time time)
{
	GenerateMonsters(time);
	MoveMonsters(time);

	for (int i = 0; i < monsters.size(); i++)
	{
		if (monsters[i]->IsDead == true)
		{
			monsters.erase(monsters.begin() + i);
		}
	}
}

void GameState::GenerateMonsters(sf::Time time)
{
	if (timeBetweenWaves < 0)
	{
		if (timeCooldownInWave < 0)
		{
			int randommonster = rand() % monster_types.size();

			if (monster_types[randommonster]->IsPassive)
			{
				monsters.emplace_back(std::make_unique<MonsterPassive>(monster_types[randommonster]->MonsterTex, monster_types[randommonster]->hp, monster_types[randommonster]->Speed, TilesSize, Health, paths_startpoints, paths_endpoints, paths, MonsterID));
				MonsterID++;
			}
			else
			{
				//TODO
			}
			MonsterNumberInWave--;
			if (MonsterNumberInWave == 0)
			{
				timeBetweenWaves = 5 + rand() % 11;
				MonsterNumberInWave = 1 + rand() % 5;
			}
			timeCooldownInWave = 0.5;
		}
		else
		{
			timeCooldownInWave = timeCooldownInWave - time.asSeconds();
		}
	}
	else
	{
		timeBetweenWaves = timeBetweenWaves - time.asSeconds();
	}
}

void GameState::MoveMonsters(sf::Time time)
{
	for (int i = 0; i < monsters.size(); i++)
	{
		monsters[i]->MonsterUpdate(time);
	}
}
