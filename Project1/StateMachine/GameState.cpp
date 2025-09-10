#include "GameState.h"
#include "StateMachine.h"

GameState::GameState()
{
	for (int i = 0; i < MapSize[0]; i++)
	{
		for (int j = 0; j < MapSize[1]; j++)
		{
			bool IsDark = (i + j) % 2;
			TowerTile tile = { sf::Vector2i(i, j), 0, IsDark};
			tower_tile.push_back(tile);
		}
	}
	std::cout << tower_tile.size();

	//road tiles
	std::string Source_path = "Resources/Visuals/Road";

	std::vector<std::string> road_source_paths;

	for (const auto& entry : std::filesystem::directory_iterator(Source_path))
	{
		road_source_paths.push_back(entry.path().string());
	}

	std::sort(road_source_paths.begin(), road_source_paths.end());

	for (int i = 0; i < road_source_paths.size(); i++)
	{
		RoadTextures.push_back(sf::Texture(road_source_paths[i]));
		//std::cout << road_source_paths[i] << std::endl;
	}

	std::ifstream file("Resources/Content/Levels/tutorial1.json");
	file >> Level;
	file.close();

	//level road
	for (auto& [road_type, coords] : Level["Road"].items())
	{
		for (int i = 0; i < coords.size(); i++)
		{
			RoadTiles.push_back(sf::RectangleShape());
			int CurrentTileNumber = RoadTiles.size() - 1;
			RoadTiles[CurrentTileNumber].setOrigin(sf::Vector2f(TilesSize / 2, TilesSize / 2));
			RoadTiles[CurrentTileNumber].setSize(sf::Vector2f(TilesSize, TilesSize));

			//In JSON file road are marked as 0, 1, 2, etc.
			// Since game read road type and sort it alphabeticaly, it is possible to easily convert road type -> id in RoadTextures vector
			RoadTiles[CurrentTileNumber].setTexture(&RoadTextures[atoi(road_type.c_str())]);

			RoadTiles[CurrentTileNumber].setPosition(sf::Vector2f(TilesSize / 2 + coords[i][0] * TilesSize, TilesSize / 2 + coords[i][1] * TilesSize));

			//remove tile from road
			for (int j = 0; j < tower_tile.size(); j++)
			{
				if (tower_tile[j].Position.x == coords[i][0] and tower_tile[j].Position.y == coords[i][1])
				{
					tower_tile.erase(tower_tile.begin() + j);
					break;
				}
			}
		}
	}

	//Objects (TODO)

	//paths for monsters

	//Monsters
	for (auto& monster: Level["monsters"])
	{
		monster_types.push_back(monster);

		//Open, import, create class etc
	}

	//Import tower textures
	cannon_base = sf::Texture("Resources/Visuals/Towers/Cannon/cannon_base.png");
	cannon_top = sf::Texture("Resources/Visuals/Towers/Cannon/cannon_top.png");

	//Settings (TODO)
}

void GameState::Input(sf::RenderWindow& window, sf::Clock& DeltaClock)
{
	while (auto event = window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(window, *event);
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
	}

	ImGui::SFML::Update(window, DeltaClock.restart());
}

void GameState::Update(sf::Clock& DeltaClock)
{
	for (int i = 0; i < tower_tile.size(); i++)
	{
		ImGui::SetNextWindowSize(ImVec2(TilesSize, TilesSize));
		ImGui::SetNextWindowPos(ImVec2(tower_tile[i].Position.x * TilesSize, tower_tile[i].Position.y * TilesSize));
		std::string name = std::to_string(i);   //names need to be unique

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoScrollbar);
		name = name + "button";

		if (tower_tile[i].IsDarker)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.25f));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
		}

		if (ImGui::Button(("##" + name).c_str(), ImVec2(TilesSize, TilesSize)) and tower_tile[i].TowerID == 0)
		{
			towers.emplace_back(std::make_unique<CannonTower>(sf::Vector2f(tower_tile[i].Position.x * TilesSize, tower_tile[i].Position.y * TilesSize), &cannon_base, &cannon_top, TowerID));
			tower_tile[i].TowerID = TowerID;
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

	window.display();
}
