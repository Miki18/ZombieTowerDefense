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
	}

	ImGui::SFML::Update(window, time);
}

void GameState::Update(sf::Time time)
{
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
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
		}

		if (ImGui::Button(("##" + name).c_str(), ImVec2(TilesSize, TilesSize)) and grass_tile[i].TowerID == 0)
		{
			towers.emplace_back(std::make_unique<CannonTower>(sf::Vector2f(grass_tile[i].Position.x * TilesSize, grass_tile[i].Position.y * TilesSize), &cannon_base, &cannon_top, TowerID));
			grass_tile[i].TowerID = TowerID;
			TowerID++;
		}

		ImGui::PopStyleColor(1);
		ImGui::End();
		ImGui::PopStyleVar(1);
	}

	for (int i = 0; i < TowerTypes; i++)
	{
		ImGui::SetNextWindowPos(ImVec2(1600-TilesSize*TowerTypes + TilesSize*i, 850));
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		std::string name = "UI" + std::to_string(i);

		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

		name = "buttonUI" + std::to_string(i);
		
		if (i == SelectedTower)
		{
			ImGui::ImageButton(name.c_str(), UI_Sprite[1], sf::Vector2f(50, 50), sf::Color(255, 255, 255, 255));
		}
		else
		{
			if (ImGui::ImageButton(name.c_str(), UI_Sprite[0], sf::Vector2f(50, 50), sf::Color(255, 255, 255, 255)))
			{
				SelectedTower = i;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar(1);
	}

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
			monsters.emplace_back(std::make_unique<MonsterPassive>(ZombieTex, 100, 100, TilesSize, paths_startpoints, paths_endpoints, paths)); //replace later with real data
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
