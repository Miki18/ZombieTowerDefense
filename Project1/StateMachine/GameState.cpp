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

	tower_options.circle.setOutlineColor(sf::Color::Red);
	tower_options.circle.setFillColor(sf::Color(255, 0, 0, 60));

	if (!speech.empty())
	{
		IsGamePaused = true;
		ShowMessage = true;
	}
}

void GameState::ExitFuntion()
{
	StateMachine::Get().ChangeState(new MenuState());
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
					ShowMessage = false;

					if (PlayerWin)
					{
						ExitFuntion();
					}
					if (PlayerLose)
					{
						ExitFuntion();
					}
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

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				tower_options.IsVisible = false;
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
		ImGui::SetNextWindowPos(ImVec2(grass_tile[i].Position.x * TilesSize, grass_tile[i].Position.y * TilesSize));  //calculate position in screen (1600 x 900)
		std::string name = std::to_string(i);   //names need to be unique

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoScrollbar);
		name = name + "button";

		if (grass_tile[i].IsDarker)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.25f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.60f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.45f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
		}

		if (ImGui::Button(("##" + name).c_str(), ImVec2(TilesSize, TilesSize)) and IsGamePaused == false)
		{
			if (grass_tile[i].TowerID == 0)
			{
				if (SelectedTower == 0 and Money >= towersvalues[0]->price)
				{
					//when creating tower calculate X and Y position on screen (top left corner of GrassTile)
					towers.emplace_back(std::make_unique<CannonTower>(
						sf::Vector2f(grass_tile[i].Position.x * TilesSize, grass_tile[i].Position.y * TilesSize),
						towersvalues[0]->hp, towersvalues[0]->cooldown, towersvalues[0]->dmg, towersvalues[0]->radius, towersvalues[0]->bulletoffset, &towersvalues[0]->base, &towersvalues[0]->top, TowerID));
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
					if (towers[j]->getID() == T_ID)
					{
						tower_options.IsVisible = true;
						float radius = towers[j]->getRadius();
						tower_options.circle.setRadius(radius);
						tower_options.circle.setOrigin(sf::Vector2f(radius, radius));
						tower_options.circle.setPosition(towers[j]->getPosition());
						break;
					}
				}
			}
		}

		ImGui::PopStyleColor(3);
		ImGui::End();
		ImGui::PopStyleVar(1);
	}

	//Select tower UI
	//SelectTowerUI();

	//Heart and Money UI
	ShowHealtAndMoney();

	if (IsGamePaused == true)
	{
		ShowPauseMessage();
		return;
	}

	//Tower
	UpdateTowers(time);

	//Update Monster
	UpdateMonsters(time);

	//Handle bullets
	UpdateBullets(time);
	Bullet_MonsterCollision();
	RemoveUnusedBullets();

	if (Health <= 0)
	{
		PlayerLose = true;
		IsGamePaused = true;
	}

	if (LevelTime > 0)
	{
		LevelTime = LevelTime - time.asSeconds();
	}
	else
	{
		if (monsters.size() == 0 and PlayerLose == false)
		{
			PlayerWin = true;
			IsGamePaused = true;
		}
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
	
	if(tower_options.IsVisible==true)
	{
		window.draw(tower_options.circle);
	}

	for (int i = 0; i < towers.size(); i++)
	{
		towers[i]->draw(window);
	}

	for (int i = 0; i < monsters.size(); i++)
	{
		monsters[i]->DrawMonster(window);
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i].DrawBullet(window);
	}

	for (int i = 0; i < monsters.size(); i++)
	{
		monsters[i]->DrawHealth(window);
	}

	window.display();
}

void GameState::UpdateTowers(sf::Time time)
{
	for (int i = 0; i < towers.size(); i++)
	{
		towers[i]->UpdateTower(time);
		DetectEnemies(i);
	}
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
	cursorPos = ImVec2(cursorPos.x + TilesSize * 2 / 2 - TextSize.x / 2, cursorPos.y - TilesSize / 2 - TextSize.y);
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

void GameState::ShowPauseMessage()
{
	sf::Vector2f picSize = { 310,210 };

	ImGui::SetNextWindowSize(ImVec2(picSize.x, picSize.y));
	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2));

	ImGui::Begin("PausedGameText", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::Image(UI_Sprite[SpriteList(UI_BigRectangle)]);

	ImDrawList* drawlist = ImGui::GetForegroundDrawList();
	if (ShowMessage)
	{
		for (int i = 0; i < speech.size() and i < 9; i++)
		{
			ImVec2 T_size = ImGui::CalcTextSize(speech[i].c_str());
			ImVec2 cursorPos = ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2 + 20);
			cursorPos = ImVec2(cursorPos.x + picSize.x / 2 - T_size.x / 2, cursorPos.y + T_size.y * i + 5 * i);
			drawlist->AddText(cursorPos, IM_COL32_WHITE, speech[i].c_str());
		}

		ImVec2 T_size = ImGui::CalcTextSize("Press P to start");
		ImVec2 cursorPos = ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2 + 20);
		cursorPos = ImVec2(cursorPos.x + picSize.x / 2 - T_size.x / 2, cursorPos.y + T_size.y * 9 + 5 * 9);
		drawlist->AddText(cursorPos, IM_COL32_WHITE, "Press P to start");
	}
	else if(PlayerWin)
	{
		ImVec2 T_size = ImGui::CalcTextSize("Congrats! You win!");
		ImVec2 cursorPos = ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2);
		cursorPos = ImVec2(cursorPos.x + picSize.x / 2 - T_size.x / 2, cursorPos.y + picSize.y / 2 - T_size.y);
		drawlist->AddText(cursorPos, IM_COL32_WHITE, "Congrats! You win!");

		T_size = ImGui::CalcTextSize("Click P to continue");
		cursorPos = ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2);
		cursorPos = ImVec2(cursorPos.x + picSize.x / 2 - T_size.x / 2, cursorPos.y + picSize.y / 2 + T_size.y);
		drawlist->AddText(cursorPos, IM_COL32_WHITE, "Click P to continue");
	}
	else if(PlayerLose)
	{
		ImVec2 T_size = ImGui::CalcTextSize("You lose!");
		ImVec2 cursorPos = ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2);
		cursorPos = ImVec2(cursorPos.x + picSize.x / 2 - T_size.x / 2, cursorPos.y + picSize.y / 2 - T_size.y);
		drawlist->AddText(cursorPos, IM_COL32_WHITE, "You lose!");

		T_size = ImGui::CalcTextSize("Click P to continue");
		cursorPos = ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2);
		cursorPos = ImVec2(cursorPos.x + picSize.x / 2 - T_size.x / 2, cursorPos.y + picSize.y / 2 + T_size.y);
		drawlist->AddText(cursorPos, IM_COL32_WHITE, "Click P to continue");
	}
	else
	{
		ImVec2 T_size = ImGui::CalcTextSize("Game is Paused");
		ImVec2 cursorPos = ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2);
		cursorPos = ImVec2(cursorPos.x + picSize.x / 2 - T_size.x / 2, cursorPos.y + picSize.y / 2 - T_size.y);
		drawlist->AddText(cursorPos, IM_COL32_WHITE, "Game is Paused");

		T_size = ImGui::CalcTextSize("Click P to resume");
		cursorPos = ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2);
		cursorPos = ImVec2(cursorPos.x + picSize.x / 2 - T_size.x / 2, cursorPos.y + picSize.y / 2 + T_size.y);
		drawlist->AddText(cursorPos, IM_COL32_WHITE, "Click P to resume");
	}

	ImGui::End();
	return;
}

void GameState::DetectEnemies(int tower_number)
{
	for (int j = 0; j < monsters.size(); j++)
	{
		//return direction (normalized vector) to target or sf::Vector2f(0,0) if tower has not enough range
		sf::Vector2f Dir = towers[tower_number]->detectEnemy(monsters[j]->getPosition());
		if (Dir != sf::Vector2f(0,0))
		{
			//if there will be more then one type of shooting (like constant shooting) you will have to place if(type_shooting == "type") {do this}
			//Can shoot also check tower's cooldown
			if (towers[tower_number]->CanShoot())
			{
				bullets.emplace_back(towers[tower_number]->getBulletStartingPosition(Dir), towers[tower_number]->getDmg(), monsters[j]->getPosition(), monsters[j]->GetID());
			}
			break;
		}
	}
}

void GameState::UpdateBullets(sf::Time time)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		bool MonsterSelected = false;
		for (int j = 0; j < monsters.size(); j++)
		{
			if (monsters[j]->GetID() == bullets[i].getTargetID())
			{
				MonsterSelected = true;
				sf::Vector2f Dir = monsters[j]->getPosition() - bullets[i].getPosition();
				float length = Dir.length();
				Dir.x = Dir.x / length;
				Dir.y = Dir.y / length;

				bullets[i].Update(time, Dir, monsters[j]->getPosition());
				break;
			}
		}

		if (MonsterSelected == false)
		{
			sf::Vector2f Dir = bullets[i].getTargetPos() - bullets[i].getPosition();
			float length = Dir.length();
			Dir.x = Dir.x / length;
			Dir.y = Dir.y / length;

			bullets[i].Update(time, Dir, bullets[i].getTargetPos());
		}
	}
}

void GameState::Bullet_MonsterCollision()
{
	std::vector<Bullet>::iterator B_iter = bullets.begin();
	std::vector<std::unique_ptr<Monster>>::iterator M_iter = monsters.begin();

	for (B_iter = bullets.begin(); B_iter != bullets.end();)
	{
		bool hit = false;
		for (M_iter = monsters.begin(); M_iter != monsters.end();)
		{
			if (B_iter->getRadius() > ((*M_iter)->getPosition() - B_iter->getPosition()).length())
			{
				hit = true;
				(*M_iter)->TakeDamage(B_iter->getDmg());
				B_iter = bullets.erase(B_iter);
				if ((*M_iter)->IsDead == true)
				{
					monsters.erase(M_iter);
				}
				break;
			}
			M_iter++;
		}

		if (hit == false)
		{
			B_iter++;
		}
	}
}

void GameState::RemoveUnusedBullets()
{
	//In case if bullet reach target position, and monster (target) is already gone (was killed by another bullet) then remove this bullet
	std::vector<Bullet>::iterator B_iter;

	for (B_iter = bullets.begin(); B_iter != bullets.end();)
	{
		if ((B_iter->getPosition() - B_iter->getTargetPos()).length() < B_iter->getRadius())
		{
			B_iter = bullets.erase(B_iter);
			continue;
		}
		B_iter++;
	}
}

void GameState::UpdateMonsters(sf::Time time)
{
	if (LevelTime > 0)
	{
		GenerateMonsters(time);
	}
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
	if (MWS.timeBetweenWaves < 0)
	{
		if (MWS.timeCooldownInWave < 0)
		{
			int randommonster = rand() % monster_types.size();

			if (monster_types[randommonster]->IsPassive)
			{
				monsters.emplace_back(std::make_unique<MonsterPassive>(monster_types[randommonster]->MonsterTex, monster_types[randommonster]->hp, monster_types[randommonster]->Speed, monster_types[randommonster]->price, TilesSize, Health, Money, paths_startpoints, paths_endpoints, paths, MonsterID));
				MonsterID++;
			}
			else
			{
				//TODO
			}
			MWS.MonsterNumberInCurrentWave--;
			if (MWS.MonsterNumberInCurrentWave == 0)
			{
				MWS.PossibleAdditionalMonsters += MWS.IncreasingPossibleNumber;
				MWS.MinimumMonstersInWave += MWS.IncreasingMinimumNumber;
				MWS.MonsterNumberInCurrentWave = MWS.MinimumMonstersInWave + rand() % int(MWS.PossibleAdditionalMonsters);
				MWS.timeBetweenWaves = MWS.BetweenWaves;
			}
			MWS.timeCooldownInWave = MWS.CooldownInWave;
		}
		else
		{
			MWS.timeCooldownInWave = MWS.timeCooldownInWave - time.asSeconds();
		}
	}
	else
	{
		MWS.timeBetweenWaves = MWS.timeBetweenWaves - time.asSeconds();
	}
}

void GameState::MoveMonsters(sf::Time time)
{
	for (int i = 0; i < monsters.size(); i++)
	{
		monsters[i]->MonsterUpdate(time);
	}
}
