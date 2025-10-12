//Load Functions-------------------------------
void GameState::GenerateGrassTiles()
{
	for (int i = 0; i < MapSize[0]; i++)
	{
		for (int j = 0; j < MapSize[1]; j++)
		{
			bool IsDark = (i + j) % 2;
			GrassTile tile = { sf::Vector2i(i, j), 0, IsDark };
			grass_tile.push_back(tile);
		}
	}
}

void GameState::LoadRoadTiles()
{
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
			for (int j = 0; j < grass_tile.size(); j++)
			{
				if (grass_tile[j].Position.x == coords[i][0] and grass_tile[j].Position.y == coords[i][1])
				{
					grass_tile.erase(grass_tile.begin() + j);
					break;
				}
			}
		}
	}
}

void GameState::LoadLevelData()
{
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
}

void GameState::LoadObjects()
{
	//TODO
}

void GameState::LoadPaths()
{
	for (const auto& arr : Level["paths"]["startpoints"])
	{
		paths_startpoints.emplace_back(sf::Vector2i(arr[0], arr[1]));
	}

	for (const auto& arr : Level["paths"]["endpoints"])
	{
		paths_endpoints.emplace_back(sf::Vector2i(arr[0], arr[1]));
	}

	for (const auto& [f, values] : Level["paths"]["path"].items())
	{
		PathPoints newPoint;
		auto pos = f.find(',');
		int x = std::stoi(f.substr(0, pos));
		int y = std::stoi(f.substr(pos + 1));

		newPoint.point = sf::Vector2i(x, y);

		std::vector<sf::Vector2i> target;

		for (const auto& arr : values)
		{
			target.emplace_back(sf::Vector2i(arr[0], arr[1]));
		}

		newPoint.successors = target;

		paths.emplace_back(newPoint);
	}
}

void GameState::LoadMonsters()
{
	for (auto& monster : Level["monsters"])
	{
		MonsterTypeValues nextmonster;

		std::string path_to_tex = "Resources/Visuals/Monsters/" + monster.get<std::string>() + "_sheet.png";

		nextmonster.MonsterTex.loadFromFile(path_to_tex);

		nlohmann::json monstervalues;
		std::string path_to_json = "Resources/Content/Monsters/" + monster.get<std::string>();
		std::fstream file(path_to_json);
		file >> monstervalues;
		file.close();

		nextmonster.hp = monstervalues["hp"].get<float>();
		nextmonster.Speed = monstervalues["speed"].get<float>();
		nextmonster.IsPassive = monstervalues["type"].get<std::string>() == "passive";
		nextmonster.price = monstervalues["price"].get<int>();

		monster_types.emplace_back(std::make_unique<MonsterTypeValues>(nextmonster));
	}
}

void GameState::LoadTowerTextures()
{
	TowerTypeValues towertype;

	towertype.base.loadFromFile("Resources/Visuals/Towers/Cannon/cannon_base.png");
	towertype.base.setSmooth(true);
	towertype.top.loadFromFile("Resources/Visuals/Towers/Cannon/cannon_top.png");
	towertype.top.setSmooth(true);

	std::fstream file("Resources/Content/Towers/cannon");

	nlohmann::json readedtower;
	file >> readedtower;

	file.close();

	towertype.hp = readedtower["hp"].get<float>();
	towertype.cooldown = readedtower["cooldown"].get<float>();
	towertype.dmg = readedtower["dmg"].get<float>();
	towertype.price = readedtower["price"].get<int>();
	towertype.radius = readedtower["radius"].get<float>();
	towertype.bulletoffset = readedtower["bulletoffset"].get<float>();

	towersvalues.emplace_back(std::make_unique<TowerTypeValues>(towertype));
}

void GameState::LoadSettings()
{
	MWS.MinimumMonstersInWave = Level["settings"]["MinimumMonstersInWave"].get<int>();
	MWS.PossibleAdditionalMonsters = Level["settings"]["PossibleAdditionalMonsters"].get<int>() + 1;
	MWS.timeBetweenWaves = Level["settings"]["timeBetweenWaves"].get<float>();
	MWS.timeCooldownInWave = Level["settings"]["timeCooldownInWave"].get<float>();
	MWS.MonsterNumberInCurrentWave = MWS.MinimumMonstersInWave + rand() % MWS.PossibleAdditionalMonsters;
	MWS.BetweenWaves = MWS.timeBetweenWaves;
	MWS.CooldownInWave = MWS.timeCooldownInWave;
}
//End loading functions---------------------------