#include "GameState.h"
#include "StateMachine.h"
#include "LoadLevel.inl"

GameState::GameState() : Music("Resources/Sounds/Carefree.mp3")
{
	GenerateGrassTiles();

	LoadLevelData();

	LoadRoadTiles();

	LoadPaths();

	LoadMonsters();

	LoadTowerTextures();

	LoadSettings();

	LoadMonsterWaves();

	LoadGold();

	LoadGoldTexture();

	LoadSounds();

	tower_options.circle.setOutlineColor(sf::Color::Red);
	tower_options.circle.setFillColor(sf::Color(255, 0, 0, 60));

	preview.Small.setFillColor(sf::Color(0, 0, 255, 80));
	preview.Big.setFillColor(sf::Color(255, 255, 0, 80));

	CalculatePreview();

	if (!speech.empty())
	{
		IsGamePaused = true;
		IsMessageVisible = true;
	}

	Music.play();
	Music.setVolume(StateMachine::Get().GetVolume(1));
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
					IsMessageVisible = false;

					if (IsPlayerWin)
					{
						ExitFuntion();
					}
					if (IsPlayerLose)
					{
						ExitFuntion();
					}
				}
				if (keyPressed->scancode == sf::Keyboard::Scancode::Num1)
				{
					SelectedTower = 0;
					CalculatePreview();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2)
				{
					SelectedTower = 1;
					CalculatePreview();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num3)
				{
					SelectedTower = 2;
					CalculatePreview();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Tab)
				{
					SelectedTower = -1;
					CalculatePreview();
				}
				
				if (keyPressed->scancode == sf::Keyboard::Scancode::Q)
				{
					tower_options.IsVisible = false;
					preview.ShowPreview = true;
				}
			}

			if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
			{
				if (keyReleased->scancode == sf::Keyboard::Scancode::Q)
				{
					preview.ShowPreview = false;
				}
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
			{
				tower_options.IsVisible = false;
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				if (SelectedTower == -1 and Money >= 100)
				{
					for (int i = 0; i < RoadTiles.size(); i++)
					{
						if (RoadTiles[i].IsBridge == true and RoadTiles[i].shape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)))
						{
							Money = Money - 100;
							RemoveBridge(i);
							break;
						}
					}
				}
			}
		}
	}

	ImGui::SFML::Update(window, time);
}

void GameState::Update(sf::RenderWindow& window, sf::Time time)
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
			if (grass_tile[i].TowerID == 0 and grass_tile[i].HasRuins == false)
			{
				if (SelectedTower == 0 and Money >= towersvalues[0]->price)
				{
					//when creating tower calculate X and Y position on screen (top left corner of GrassTile)
					towers.emplace_back(std::make_unique<CannonTower>(
						sf::Vector2f(grass_tile[i].Position.x * TilesSize, grass_tile[i].Position.y * TilesSize),
						towersvalues[0]->hp, towersvalues[0]->IncreaseHp, 
						towersvalues[0]->cooldown, towersvalues[0]->IncreaseCooldown, 
						towersvalues[0]->dmg, towersvalues[0]->IncreaseDmg, 
						towersvalues[0]->radius, towersvalues[0]->IncreaseRadius, 
						towersvalues[0]->bulletoffset, towersvalues[0]->bulletspeed,
						&towersvalues[0]->base, &towersvalues[0]->top, TowerID,
						towersvalues[0]->price, towersvalues[0]->UpgradePrice, towersvalues[0]->IncreaseUpgradePrice, TowerSound));
					grass_tile[i].TowerID = TowerID;
					Money = Money - towersvalues[0]->price;
					TowerID++;

					tower_options.IsVisible = true;
					float radius = towers[towers.size()-1]->getRadius();
					tower_options.circle.setRadius(radius);
					tower_options.circle.setOrigin(sf::Vector2f(radius, radius));
					tower_options.circle.setPosition(towers[towers.size()-1]->getPosition());
					tower_options.SelectedTowerID = towers[towers.size() - 1]->getID();

				}
				else if (SelectedTower == 1 and Money >= towersvalues[1]->price)
				{
					towers.emplace_back(std::make_unique<SniperTower>(
						sf::Vector2f(grass_tile[i].Position.x * TilesSize, grass_tile[i].Position.y * TilesSize),
						towersvalues[1]->hp, towersvalues[1]->IncreaseHp,
						towersvalues[1]->cooldown, towersvalues[1]->IncreaseCooldown,
						towersvalues[1]->dmg, towersvalues[1]->IncreaseDmg,
						towersvalues[1]->radius, towersvalues[1]->IncreaseRadius,
						towersvalues[1]->bulletoffset, towersvalues[1]->bulletspeed,
						&towersvalues[1]->base, &towersvalues[1]->top, TowerID,
						towersvalues[1]->price, towersvalues[1]->UpgradePrice, towersvalues[1]->IncreaseUpgradePrice, TowerSound));
					grass_tile[i].TowerID = TowerID;
					Money = Money - towersvalues[1]->price;
					TowerID++;

					tower_options.IsVisible = true;
					float radius = towers[towers.size() - 1]->getRadius();
					tower_options.circle.setRadius(radius);
					tower_options.circle.setOrigin(sf::Vector2f(radius, radius));
					tower_options.circle.setPosition(towers[towers.size() - 1]->getPosition());
					tower_options.SelectedTowerID = towers[towers.size() - 1]->getID();
				}
				else if (SelectedTower == 2 and Money >= towersvalues[2]->price and grass_tile[i].HasGold == true)
				{
					towers.emplace_back(std::make_unique<Goldmine>(
						sf::Vector2f(grass_tile[i].Position.x * TilesSize, grass_tile[i].Position.y * TilesSize),
						towersvalues[2]->hp, towersvalues[2]->IncreaseHp,
						towersvalues[2]->cooldown, towersvalues[2]->IncreaseCooldown,
						towersvalues[2]->dmg, towersvalues[2]->IncreaseDmg,
						towersvalues[2]->radius, towersvalues[2]->IncreaseRadius,
						towersvalues[2]->bulletoffset, towersvalues[2]->bulletspeed,
						&towersvalues[2]->base, &towersvalues[2]->top, TowerID,
						towersvalues[2]->price, towersvalues[2]->UpgradePrice, towersvalues[2]->IncreaseUpgradePrice, TowerSound));
					grass_tile[i].TowerID = TowerID;
					Money = Money - towersvalues[2]->price;
					TowerID++;

					tower_options.IsVisible = true;
					float radius = towers[towers.size() - 1]->getRadius();
					tower_options.circle.setRadius(radius);
					tower_options.circle.setOrigin(sf::Vector2f(radius, radius));
					tower_options.circle.setPosition(towers[towers.size() - 1]->getPosition());
					tower_options.SelectedTowerID = towers[towers.size() - 1]->getID();
				}
			}
			else if (grass_tile[i].TowerID == 0 and grass_tile[i].HasRuins == true)
			{
				if (Money >= 100 and SelectedTower == -1)
				{
					Money = Money - 100;
					grass_tile[i].HasRuins = false;
				}
			}
			else if (grass_tile[i].TowerID != 0)
			{
				tower_options.SelectedTowerID = grass_tile[i].TowerID;
				for (int j = 0; j < towers.size(); j++)
				{
					if (towers[j]->getID() == tower_options.SelectedTowerID)
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

	SelectTowerUI();

	ShowHealtAndMoney();

	if (IsGamePaused == true)
	{
		ShowMessage();
		return;
	}

	if (tower_options.IsVisible == true)
	{
		for (int i = 0; i < towers.size(); i++)
		{
			if (towers[i]->getID() == tower_options.SelectedTowerID)
			{
				TowerUI(i);
				break;
			}
		}
	}

	if (preview.ShowPreview == true)
	{
		UpdatePreview();
	}

	UpdateTowers(time);

	UpdateMonsters(time);

	UpdateBullets(time);
	Bullet_ObjectsCollision();
	RemoveUnusedBullets();

	if (Health <= 0)
	{
		IsPlayerLose = true;
		IsGamePaused = true;
	}

	if (monsters.size() == 0 and IsPlayerLose == false and wave_number == MWS.size())
	{	
		Music.stop();
		SaveProgress();
		IsPlayerWin = true;
		IsGamePaused = true;
	}
}

void GameState::Render(sf::RenderWindow& window)
{
	window.clear(sf::Color{60, 160, 0, 255}); //Background color RGBA

	for (int i = 0; i < RoadTiles.size(); i++)
	{
		window.draw(RoadTiles[i].shape);
	}

	for (int i = 0; i < BlackHole.size(); i++)
	{
		window.draw(BlackHole[i]);
	}

	for (int i = 0; i < RedArrows.size(); i++)
	{
		window.draw(RedArrows[i]);
	}

	ImGui::SFML::Render(window);
	
	if (IsGamePaused == true)
	{
		window.display();

		return;
	}

	if (tower_options.IsVisible == true)
	{
		window.draw(tower_options.circle);
	}

	if (preview.ShowPreview == true)
	{
		window.draw(preview.Big);
		window.draw(preview.Small);
	}

	for (int i = 0; i < GoldBars.size(); i++)
	{
		window.draw(GoldBars[i]);
	}

	for (int i = 0; i < grass_tile.size(); i++)
	{
		if (grass_tile[i].HasRuins == true)
		{
			sf::Sprite sprite(RuinsTex);
			sprite.setPosition(sf::Vector2f(grass_tile[i].Position.x * TilesSize, grass_tile[i].Position.y * TilesSize));
			sprite.setScale(sf::Vector2f(50.0f / 64.0f, 50.0f / 64.0f));
			window.draw(sprite);
		}
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

	for (int i = 0; i < towers.size(); i++)
	{
		towers[i]->drawhealth(window);
	}

	window.display();
}

void GameState::UpdateTowers(sf::Time time)
{
	for (int i = 0; i < towers.size(); i++)
	{
		towers[i]->UpdateTower(time);
		DetectEnemies(i);
		towers[i]->GenerateGold(&Money);
	}
}

void GameState::RemoveBridge(int index)
{
	//get location
	sf::Vector2f Pos = RoadTiles[index].shape.getPosition();
	sf::Vector2i location = { int(Pos.x) / TilesSize, int(Pos.y) / TilesSize };

	//Replace nearest road tiles
	for (int i = 0; i < RoadTiles.size(); i++)
	{
		if (RoadTiles[i].shape.getPosition() == sf::Vector2f(Pos.x - TilesSize, Pos.y))
		{
			if (RoadTiles[i].shape.getTexture() == &RoadTextures[3])  //left right down
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[0]);
			}

			else if (RoadTiles[i].shape.getTexture() == &RoadTextures[4]) //all directions
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[1]);
			}

			else if (RoadTiles[i].shape.getTexture() == &RoadTextures[5]) //left right top
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[6]);
			}

			else if (RoadTiles[i].shape.getTexture() == &RoadTextures[8]) //right down top
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[10]);
			}
		}
		if (RoadTiles[i].shape.getPosition() == sf::Vector2f(Pos.x + TilesSize, Pos.y))
		{
			if (RoadTiles[i].shape.getTexture() == &RoadTextures[3]) //left right down
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[7]);
			}

			else if (RoadTiles[i].shape.getTexture() == &RoadTextures[4]) //all directions
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[8]);
			}

			else if (RoadTiles[i].shape.getTexture() == &RoadTextures[5]) //left right top
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[8]);
			}

			else if (RoadTiles[i].shape.getTexture() == &RoadTextures[2]) //left top down
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[10]);
			}
		}
		if (RoadTiles[i].shape.getPosition() == sf::Vector2f(Pos.x, Pos.y + TilesSize))
		{
			if (RoadTiles[i].shape.getTexture() == &RoadTextures[1]) //left down top
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[6]);
			}

			else if (RoadTiles[i].shape.getTexture() == &RoadTextures[4]) //all directions
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[5]);
			}

			else if (RoadTiles[i].shape.getTexture() == &RoadTextures[3]) //left right down
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[7]);
			}

			else if (RoadTiles[i].shape.getTexture() == &RoadTextures[8]) //right top down
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[9]);
			}
		}
		if (RoadTiles[i].shape.getPosition() == sf::Vector2f(Pos.x, Pos.y - TilesSize))
		{
			if (RoadTiles[i].shape.getTexture() == &RoadTextures[1]) //left down top
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[0]);
			}

			else if (RoadTiles[i].shape.getTexture() == &RoadTextures[4]) //all directions
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[3]);
			}

			else if (RoadTiles[i].shape.getTexture() == &RoadTextures[5]) //left right top
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[2]);
			}

			else if (RoadTiles[i].shape.getTexture() == &RoadTextures[8]) //right top down
			{
				RoadTiles[i].shape.setTexture(&RoadTextures[7]);
			}
		}
	}

	//create imgui grass tile
	bool IsDark = (location.x + location.y) % 2;
	GrassTile tile = { sf::Vector2i(location.x, location.y), 0, IsDark };
	grass_tile.push_back(tile);

	//Remove this segment from path tile
	for (int i = 0; i < paths.size(); i++)
	{
		for (int j = 0; j < paths[i].successors.size(); j++)
		{
			if (paths[i].successors[j] == location)
			{
				paths[i].successors.erase(paths[i].successors.begin() + j);
				j = j - 1;
			}
		}
	}

	//erase road
	RoadTiles.erase(RoadTiles.begin() + index);
}

void GameState::ShowHealtAndMoney()
{
	//health
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::SetNextWindowSize(ImVec2(TilesSize, TilesSize));
	ImGui::SetNextWindowPos(ImVec2(0, ScreenSize[1] - TilesSize));
	ImGui::Begin("heart", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	ImGui::Image(sf::Sprite(UI_Sprite[SpriteList(UI_Heart)]), sf::Vector2f(TilesSize, TilesSize), sf::Color(255, 255, 255, 255));
	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	ImDrawList* drawlist = ImGui::GetWindowDrawList();
	ImVec2 TextSize = ImGui::CalcTextSize(std::to_string(Health).c_str());
	cursorPos = ImVec2(cursorPos.x + TilesSize / 2 - TextSize.x / 2, cursorPos.y - TilesSize / 2 - TextSize.y);
	drawlist->AddText(cursorPos, IM_COL32_WHITE, std::to_string(Health).c_str());
	ImGui::End();

	//Money
	ImGui::SetNextWindowPos(ImVec2(TilesSize, ScreenSize[1] - TilesSize));
	ImGui::Begin("Money", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	ImGui::Image(sf::Sprite(UI_Sprite[SpriteList(UI_Money)]), sf::Vector2f(TilesSize * 2, TilesSize), sf::Color(255, 255, 255, 255));
	cursorPos = ImGui::GetCursorScreenPos();
	drawlist = ImGui::GetWindowDrawList();
	TextSize = ImGui::CalcTextSize(std::to_string(Money).c_str());
	cursorPos = ImVec2(cursorPos.x + TilesSize * 2 / 2 - TextSize.x / 2, cursorPos.y - TilesSize / 2 - TextSize.y);
	drawlist->AddText(cursorPos, IM_COL32_WHITE, std::to_string(Money).c_str());
	ImGui::End();

	//Hammer
	ImGui::SetNextWindowSize(ImVec2(TilesSize, TilesSize));
	ImGui::SetNextWindowPos(ImVec2(3 * TilesSize, ScreenSize[1] - TilesSize));

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0, 0.0, 0.0, 0.0));

	bool IsHammerSelected = false;

	if (SelectedTower == -1)
	{
		IsHammerSelected = true;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
	}

	ImGui::Begin("Hammer", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	if (ImGui::ImageButton("HammerButton", UI_Sprite[SpriteList(UI_Hammer)], sf::Vector2f(TilesSize, TilesSize)))
	{
		SelectedTower = -1;
	}

	if (ImGui::IsItemHovered() == true)
	{
		ImVec2 Txt = ImGui::CalcTextSize("Price:");
		ImGui::SetCursorScreenPos(ImVec2(3 * TilesSize + (TilesSize - Txt.x) / 2, ScreenSize[1] - TilesSize / 2 - price_offset));
		ImGui::Text("Price:");
		Txt = ImGui::CalcTextSize("100");
		ImGui::SetCursorScreenPos(ImVec2(3 * TilesSize + (TilesSize - Txt.x) / 2, ScreenSize[1] - TilesSize / 2 + price_offset));
		ImGui::Text("100");
	}

	ImGui::End();
	
	if (IsHammerSelected == true)
	{
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);
	}

	ImGui::PopStyleColor(4);

	ImGui::PopStyleVar(2);
}

void GameState::SelectTowerUI()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.240, 0.140, 1.0));

	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2, 0.1, 0.1, 1.0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4);

	ImGui::SetNextWindowSize(ImVec2(TilesSize, TilesSize));

	for (int i = 0; i < TowerTypes; i++)
	{
		ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] - TilesSize * TowerTypes + TilesSize * i, ScreenSize[1] - TilesSize));

		std::string name = "UI" + std::to_string(i);
		std::string button_name = "##Button" + name;

		bool IsSelected = false;

		if (SelectedTower == i)
		{
			IsSelected = true;
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
		}

		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

		if (ImGui::ImageButton(button_name.c_str(), sf::Sprite(towersvalues[i]->top), sf::Vector2f(TilesSize, TilesSize)))
		{
			SelectedTower = i;
			CalculatePreview();
		}

		if (ImGui::IsItemHovered() == true)
		{
			ImVec2 Txt = ImGui::CalcTextSize(towersvalues[i]->name.c_str());
			ImGui::SetCursorScreenPos(ImVec2(ScreenSize[0] - TilesSize * TowerTypes + TilesSize * i + (TilesSize - Txt.x) / 2, ScreenSize[1] - TilesSize / 2 - 3*price_offset));
			ImGui::Text(towersvalues[i]->name.c_str());
			Txt = ImGui::CalcTextSize("Price:");
			ImGui::SetCursorScreenPos(ImVec2(ScreenSize[0] - TilesSize * TowerTypes + TilesSize * i + (TilesSize - Txt.x) / 2, ScreenSize[1] - TilesSize / 2 - price_offset));
			ImGui::Text("Price:");
			Txt = ImGui::CalcTextSize(std::to_string(towersvalues[i]->price).c_str());
			ImGui::SetCursorScreenPos(ImVec2(ScreenSize[0] - TilesSize * TowerTypes + TilesSize * i + (TilesSize - Txt.x) / 2, ScreenSize[1] - TilesSize / 2 + price_offset));
			ImGui::Text(std::to_string(towersvalues[i]->price).c_str());
		}

		if (IsSelected == true)
		{
			ImGui::PopStyleColor(1);
		}

		ImGui::End();
	}

	ImGui::PopStyleVar(4);
	ImGui::PopStyleColor(4);
}

void GameState::TowerUI(int towersindex)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.240, 0.140, 1.0));

	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2, 0.1, 0.1, 1.0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4);

	bool IsTowerRemoved = false;

	//Start with 5*TileSize, because for Health money and hammer 4 tiles was used
	ImGui::SetNextWindowSize(ImVec2(TilesSize, TilesSize));
	ImGui::SetNextWindowPos(ImVec2(4 * TilesSize, ScreenSize[1] - TilesSize));
	ImGui::Begin("SellTower", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	if (ImGui::Button("##SellTowerButton", ImVec2(TilesSize, TilesSize)))
	{
		for (int i = 0; i < grass_tile.size(); i++)
		{
			if (grass_tile[i].TowerID == tower_options.SelectedTowerID)
			{
				grass_tile[i].TowerID = 0;
				break;
			}
		}

		Money = Money + towers[towersindex]->getSellPrice();
		tower_options.IsVisible = false;
		IsTowerRemoved = true;
		towers.erase(towers.begin() + towersindex);
	}

	if (IsTowerRemoved)
	{
		ImGui::End();
		ImGui::PopStyleColor(4);
		ImGui::PopStyleVar(4);
		return;
	}

	if (ImGui::IsItemHovered())
	{
		std::string price = std::to_string(towers[towersindex]->getSellPrice());
		ImVec2 TextSize = ImGui::CalcTextSize("Sell:");
		ImGui::SetCursorScreenPos(ImVec2(4 * TilesSize + (TilesSize/2 - TextSize.x/2), ScreenSize[1] - TilesSize/2 - TextSize.y));
		ImGui::Text("Sell:");
		TextSize = ImGui::CalcTextSize(price.c_str());
		ImGui::SetCursorScreenPos(ImVec2(4 * TilesSize + (TilesSize / 2 - TextSize.x / 2), ScreenSize[1] - TilesSize / 2));
		ImGui::Text(price.c_str());
	}
	else
	{
		ImGui::SetCursorScreenPos(ImVec2(4 * TilesSize - 7, ScreenSize[1] - TilesSize - 7));
		ImGui::Image(UI_Sprite[SpriteList::UI_Dolar], sf::Vector2f(64,64));
	}

	ImGui::End();


	if (towers[towersindex]->hasMaxHeath() or towers[towersindex]->getRepairPrice() > Money)
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5, 0.475, 0.440, 1.0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.475, 0.440, 1.0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5, 0.475, 0.440, 1.0));
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4, 0.290, 0.190, 1.0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4, 0.290, 0.190, 1.0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.240, 0.140, 1.0));
	}

	ImGui::SetNextWindowSize(ImVec2(TilesSize, TilesSize));
	ImGui::SetNextWindowPos(ImVec2(5 * TilesSize, ScreenSize[1] - TilesSize));
	ImGui::Begin("RepairTower", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	if (ImGui::Button("##RepairTowerButton", ImVec2(TilesSize, TilesSize)))
	{
		if (!towers[towersindex]->hasMaxHeath() and towers[towersindex]->getRepairPrice() <= Money)
		{
			Money = Money - towers[towersindex]->getRepairPrice();
			towers[towersindex]->Repair();
		}
	}

	if (towers[towersindex]->hasMaxHeath())
	{
		ImVec2 TextSize = ImGui::CalcTextSize("FULL");
		ImGui::SetCursorScreenPos(ImVec2(5 * TilesSize + (TilesSize / 2 - TextSize.x / 2), ScreenSize[1] - TilesSize / 2 - TextSize.y));
		ImGui::Text("FULL");
		TextSize = ImGui::CalcTextSize("HP!");
		ImGui::SetCursorScreenPos(ImVec2(5 * TilesSize + (TilesSize / 2 - TextSize.x / 2), ScreenSize[1] - TilesSize / 2));
		ImGui::Text("HP!");
	}
	else
	{
		if (ImGui::IsItemHovered())
		{
			std::string price = std::to_string(towers[towersindex]->getRepairPrice());
			ImVec2 TextSize = ImGui::CalcTextSize("Price:");
			ImGui::SetCursorScreenPos(ImVec2(5 * TilesSize + (TilesSize / 2 - TextSize.x / 2), ScreenSize[1] - TilesSize / 2 - TextSize.y));
			ImGui::Text("Price:");
			TextSize = ImGui::CalcTextSize(std::to_string(towers[towersindex]->getRepairPrice()).c_str());
			ImGui::SetCursorScreenPos(ImVec2(5 * TilesSize + (TilesSize / 2 - TextSize.x / 2), ScreenSize[1] - TilesSize / 2));
			ImGui::Text(std::to_string(towers[towersindex]->getRepairPrice()).c_str());
		}
		else
		{
			ImGui::SetCursorScreenPos(ImVec2(5 * TilesSize, ScreenSize[1] - TilesSize));
			ImGui::Image(UI_Sprite[SpriteList::UI_Repair], sf::Vector2f(TilesSize, TilesSize));
		}
	}

	ImGui::End();

	int UpgradePrice = towers[towersindex]->getUpgradePrice();

	if (Money >= UpgradePrice and towers[towersindex]->getCurrentLevel() != 5)
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4, 0.290, 0.190, 1.0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4, 0.290, 0.190, 1.0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.240, 0.140, 1.0));
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5, 0.475, 0.440, 1.0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.475, 0.440, 1.0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5, 0.475, 0.440, 1.0));
	}

	ImGui::SetNextWindowSize(ImVec2(TilesSize, TilesSize));
	ImGui::SetNextWindowPos(ImVec2(6 * TilesSize, ScreenSize[1] - TilesSize));
	ImGui::Begin("UpgradeTower", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	if (ImGui::Button("##UpgradeTowerButton", ImVec2 (TilesSize, TilesSize)))
	{
		if (Money >= towers[towersindex]->getUpgradePrice() and towers[towersindex]->getCurrentLevel() < 5)
		{
			Money = Money - towers[towersindex]->getUpgradePrice();
			towers[towersindex]->Upgrade();
			tower_options.circle.setRadius(towers[towersindex]->getRadius());
			tower_options.circle.setOrigin(sf::Vector2f(towers[towersindex]->getRadius(), towers[towersindex]->getRadius()));
		}
	}

	if (towers[towersindex]->getCurrentLevel() == 5)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 0.2, 1.0));
		ImVec2 TextSize = ImGui::CalcTextSize("MAX");
		ImGui::SetCursorScreenPos(ImVec2(6 * TilesSize + (TilesSize / 2 - TextSize.x / 2), ScreenSize[1] - TilesSize / 2 - TextSize.y));
		ImGui::Text("MAX");
		TextSize = ImGui::CalcTextSize("LVL!");
		ImGui::SetCursorScreenPos(ImVec2(6 * TilesSize + (TilesSize / 2 - TextSize.x / 2), ScreenSize[1] - TilesSize / 2));
		ImGui::Text("LVL!");
		ImGui::PopStyleColor(1);
	}
	else
	{
		if (ImGui::IsItemHovered())
		{
			std::string price = std::to_string(towers[towersindex]->getSellPrice());
			ImVec2 TextSize = ImGui::CalcTextSize("Price:");
			ImGui::SetCursorScreenPos(ImVec2(6 * TilesSize + (TilesSize / 2 - TextSize.x / 2), ScreenSize[1] - TilesSize / 2 - TextSize.y));
			ImGui::Text("Price:");
			TextSize = ImGui::CalcTextSize(std::to_string(UpgradePrice).c_str());
			ImGui::SetCursorScreenPos(ImVec2(6 * TilesSize + (TilesSize / 2 - TextSize.x / 2), ScreenSize[1] - TilesSize / 2));
			ImGui::Text(std::to_string(UpgradePrice).c_str());
		}
		else
		{
			ImGui::SetCursorScreenPos(ImVec2(6 * TilesSize, ScreenSize[1] - TilesSize));
			ImGui::Image(UI_Sprite[SpriteList::UI_Upgrade], sf::Vector2f(TilesSize, TilesSize));
		}
	}

	ImGui::End();

	ImGui::PopStyleVar(4);
	ImGui::PopStyleColor(10);
}

void GameState::ShowMessage()
{
	sf::Vector2f picSize = { 310,210 };

	ImGui::SetNextWindowSize(ImVec2(picSize.x, picSize.y));
	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2, 0.1, 0.1, 1.0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4);

	ImGui::Begin("PausedGameText", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	if (IsMessageVisible)
	{
		for (int i = 0; i < speech.size() and i < 9; i++)
		{
			ImVec2 T_size = ImGui::CalcTextSize(speech[i].c_str());
			ImVec2 cursorPos = ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2 + 20);
			cursorPos = ImVec2(cursorPos.x + picSize.x / 2 - T_size.x / 2, cursorPos.y + T_size.y * i + 5 * i);
			ImGui::SetCursorScreenPos(cursorPos);
			ImGui::Text(speech[i].c_str());
		}

		ImVec2 T_size = ImGui::CalcTextSize("Press P to start");
		ImVec2 cursorPos = ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2 + 20);
		cursorPos = ImVec2(cursorPos.x + picSize.x / 2 - T_size.x / 2, cursorPos.y + T_size.y * 9 + 5 * 9);
		ImGui::SetCursorScreenPos(cursorPos);
		ImGui::Text("Press P to start");;
	}
	else if(IsPlayerWin)
	{
		CustomTextMessages("Congrats! You win!", "Press P to continue", picSize);
	}
	else if(IsPlayerLose)
	{
		CustomTextMessages("You lose!", "Press P to continue", picSize);
	}
	else
	{
		CustomTextMessages("Game is Paused", "Press P to resume", picSize);

		StateMachine::Get().VolumeSetting(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 150), "Sounds", 0);

		StateMachine::Get().VolumeSetting(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 200), "Music", 1);
		Music.setVolume(StateMachine::Get().GetVolume(1));
	}

	ImGui::End();

	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(2);

	return;
}

void GameState::CalculatePreview()
{
	if (SelectedTower == -1)
	{
		preview.Small.setRadius(0);
		preview.Big.setRadius(0);
	}
	else
	{
		preview.Small.setRadius(towersvalues[SelectedTower]->radius);
		preview.Big.setRadius(towersvalues[SelectedTower]->radius + towersvalues[SelectedTower]->IncreaseRadius * 4);
		preview.Small.setOrigin(preview.Small.getGeometricCenter());
		preview.Big.setOrigin(preview.Big.getGeometricCenter());
	}
}

void GameState::UpdatePreview()
{
	sf::Vector2i MousePos = sf::Mouse::getPosition();

	int X = MousePos.x - (MousePos.x % TilesSize);
	int Y = MousePos.y - (MousePos.y % TilesSize);

	MousePos.x = MousePos.x - (MousePos.x % TilesSize);
	MousePos.y = MousePos.y - (MousePos.y % TilesSize);

	MousePos.x = MousePos.x + TilesSize / 2;
	MousePos.y = MousePos.y + TilesSize / 2;

	preview.Big.setPosition(sf::Vector2f(float(MousePos.x), float(MousePos.y)));
	preview.Small.setPosition(sf::Vector2f(float(MousePos.x), float(MousePos.y)));
}

void GameState::CustomTextMessages(std::string UpperText, std::string LowerText, sf::Vector2f picSize)
{
	ImVec2 T_size = ImGui::CalcTextSize(UpperText.c_str());
	ImVec2 cursorPos = ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2);
	cursorPos = ImVec2(cursorPos.x + picSize.x / 2 - T_size.x / 2, cursorPos.y + picSize.y / 2 - T_size.y);
	ImGui::SetCursorScreenPos(cursorPos);
	ImGui::Text(UpperText.c_str());

	T_size = ImGui::CalcTextSize(LowerText.c_str());
	cursorPos = ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2);
	cursorPos = ImVec2(cursorPos.x + picSize.x / 2 - T_size.x / 2, cursorPos.y + picSize.y / 2 + T_size.y);
	ImGui::SetCursorScreenPos(cursorPos);
	ImGui::Text(LowerText.c_str());
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
				bullets.emplace_back(towers[tower_number]->getBulletStartingPosition(Dir), towers[tower_number]->getDmg(), towers[tower_number]->getBulletSpeed(), monsters[j]->getPosition(), monsters[j]->GetID(), true);
				towers[tower_number]->SetVolume(StateMachine::Get().GetVolume(0));
				towers[tower_number]->PlaySound();
			}
			break;
		}
	}
}

void GameState::UpdateBullets(sf::Time time)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		bool TargetSelected = false;

		if (bullets[i].IsFriendly)
		{
			for (int j = 0; j < monsters.size(); j++)
			{
				if (monsters[j]->GetID() == bullets[i].getTargetID())
				{
					TargetSelected = true;
					sf::Vector2f Dir = monsters[j]->getPosition() - bullets[i].getPosition();
					float length = Dir.length();
					Dir.x = Dir.x / length;
					Dir.y = Dir.y / length;

					bullets[i].Update(time, Dir, monsters[j]->getPosition());
					break;
				}
			}
		}
		else
		{
			for (int j = 0; j < towers.size(); j++)
			{
				if (towers[j]->getID() == bullets[i].getTargetID())
				{
					TargetSelected = true;
					sf::Vector2f Dir = towers[j]->getPosition() - bullets[i].getPosition();
					float length = Dir.length();
					Dir.x = Dir.x / length;
					Dir.y = Dir.y / length;

					bullets[i].Update(time, Dir, towers[j]->getPosition());
					break;
				}
			}
		}

		if (TargetSelected == false)
		{
			sf::Vector2f Dir = bullets[i].getTargetPos() - bullets[i].getPosition();
			float length = Dir.length();
			Dir.x = Dir.x / length;
			Dir.y = Dir.y / length;

			bullets[i].Update(time, Dir, bullets[i].getTargetPos());
		}
	}
}

void GameState::Bullet_ObjectsCollision()
{
	std::vector<Bullet>::iterator B_iter = bullets.begin();
	std::vector<std::unique_ptr<Monster>>::iterator M_iter = monsters.begin();
	std::vector<std::unique_ptr<Tower>>::iterator T_iter = towers.begin();

	for (B_iter = bullets.begin(); B_iter != bullets.end();)
	{
		bool hit = false;

		if (B_iter->IsFriendly == true)
		{
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
		}
		else
		{
			for (T_iter = towers.begin(); T_iter != towers.end();)
			{
				if (B_iter->getRadius() > ((*T_iter)->getPosition() - B_iter->getPosition()).length())
				{
					hit = true;
					(*T_iter)->TakeDmg(int(B_iter->getDmg()));
					B_iter = bullets.erase(B_iter);
					if ((*T_iter)->gethp() < 0)
					{

						for (int i = 0; i < grass_tile.size(); i++)
						{
							if (grass_tile[i].TowerID == (*T_iter)->getID())
							{
								if ((*T_iter)->getRadius() == 0)
								{
									grass_tile[i].HasRuins = true;
								}
								grass_tile[i].TowerID = 0;
								break;
							}
						}

						tower_options.IsVisible = false;
						towers.erase(T_iter);
					}
					break;
				}
				T_iter++;
			}
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
	GenerateMonsters(time);

	for (int i = 0; i < monsters.size(); i++)
	{
		if (monsters[i]->IsShooter())
		{
			//if monster is shooter then check if it can shot tower if yes then cooldown then shoot (and it can't move)
			bool HasTower = MonsterShoot(i);
			if (HasTower == true)
			{
				monsters[i]->ChangeAnimation(1);
				monsters[i]->LowerCooldown(time);
				monsters[i]->PlayAnimation(time);
				continue;
			}
			else
			{
				monsters[i]->ChangeAnimation(0);
				monsters[i]->ResetCooldown(0.5);
			}
		}
		monsters[i]->MonsterUpdate(time);
		monsters[i]->PlayAnimation(time);
	}

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
	if (MWS.size() <= wave_number)
	{
		return;
	}

	if (MWS[wave_number].PreWaveWaiting < 0)
	{
		if (MWS[wave_number].Cooldown < 0)
		{
			if (MWS[wave_number].monsters[0] == "zombie")
			{
				monsters.emplace_back(std::make_unique<MonsterPassive>(monster_types[0]->MonsterTex, monster_types[0]->hp, monster_types[0]->Speed, monster_types[0]->price, TilesSize, Health, Money, paths_startpoints, paths, MonsterID, monster_types[0]->texSize));
				MonsterID++;
			}
			else
			{
				monsters.emplace_back(std::make_unique<MonsterActive>(monster_types[1]->MonsterTex, monster_types[1]->hp, monster_types[1]->Speed, monster_types[1]->price, TilesSize, Health, Money, paths_startpoints, paths, MonsterID, monster_types[1]->texSize, monster_types[1]->MonsterTexAttack, monster_types[1]->range, monster_types[1]->bulletoffset, monster_types[1]->Dmg, MonsterSound));
				MonsterID++;
			}

			MWS[wave_number].monsters.erase(MWS[wave_number].monsters.begin());
			if (MWS[wave_number].monsters.size() == 0)
			{
				wave_number++;
			}
			else
			{
				MWS[wave_number].Cooldown = MWS[wave_number].BaseCooldown;
			}
		}
		else
		{
			MWS[wave_number].Cooldown = MWS[wave_number].Cooldown - time.asSeconds();
		}
	}
	else
	{
		MWS[wave_number].PreWaveWaiting = MWS[wave_number].PreWaveWaiting - time.asSeconds();
	}
}

bool GameState::MonsterShoot(int monster_index)
{
	for (int i = 0; i < towers.size(); i++)
	{
		if (sf::Vector2f(monsters[monster_index]->getPosition() - towers[i]->getPosition()).length() <= monsters[monster_index]->GetRange())
		{
			if (monsters[monster_index]->GetCooldown() <= 0.0f)
			{
				bullets.emplace_back(monsters[monster_index]->getBulletStartPosition(), monsters[monster_index]->GetDmg(), 150, towers[i]->getPosition(), towers[i]->getID(), false);
				monsters[monster_index]->ResetCooldown(1.0f);
				monsters[monster_index]->SetVolume(StateMachine::Get().GetVolume(0));
				monsters[monster_index]->PlaySound();
			}
			return true;
		}
	}
	return false;
}
