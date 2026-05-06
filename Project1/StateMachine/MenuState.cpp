#include "MenuState.h"
#include "StateMachine.h"

#include <cctype>

MenuState::MenuState()
{
	for (int i = 0; i < MapSize[0]; i++)
	{
		for (int j = 0; j < MapSize[1] + 1; j++)
		{
			bool IsDark = (i + j) % 2;
			GrassTileMenu tile = { sf::Vector2i(i, j), IsDark };
			GrassTile.push_back(tile);
		}
	}

	std::ifstream file("Resources/Content/profiles");
	file >> profiles;
	file.close();

	if (StateMachine::Get().getSelectedProfile().length() == 0)
	{
		current_screen = SelectProfile;
	}
	else
	{
		current_screen = MainMenu;
	}
}

void MenuState::Input(sf::RenderWindow& window, sf::Time time)
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

void MenuState::Update(sf::RenderWindow& window, sf::Time time)
{
	for (int i = 0; i < GrassTile.size(); i++)
	{
		ImGui::SetNextWindowSize(ImVec2(TilesSize, TilesSize));
		ImGui::SetNextWindowPos(ImVec2(GrassTile[i].Position.x * TilesSize, GrassTile[i].Position.y * TilesSize));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.23f, 0.63f, 0.0f, 1.0f));

		if (GrassTile[i].IsDarker)
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.25f));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
		}

		std::string name = std::to_string(i);

		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs);

		ImGui::End();

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(2);
	}

	switch (current_screen)
	{
	case MenuState::SelectProfile:
		SelectProfileScreen();
		break;
	case MenuState::MainMenu:
		MainMenuScreen();
		break;
	case MenuState::SelectLevel:
		SelectLevelScreen();
		break;
	case MenuState::Options:
		OptionsScreen();
		break;
	case MenuState::Settings:
		SettingsScreen(window);
		break;
	case MenuState::Credits:
		CreditsScreen();
		break;
	case MenuState::Profile:
		ProfileScreen();
		break;
	case MenuState::ProfileStats:
		ProfileStatsScreen();
		break;
	case MenuState::DeleteProfile:
		DeleteProfileScreen();
		break;
	default:
		break;
	}
}

void MenuState::Render(sf::RenderWindow& window)
{
	window.clear(sf::Color{ 60, 160, 0, 255 });

	ImGui::SFML::Render(window);
	window.display();

	if (changeMode == true)
	{
		changeMode = false;
		window.close();
		if (StateMachine::Get().IsFullscreen)
		{
			window.create(sf::VideoMode({ 1600, 900 }), "Game", sf::Style::None, sf::State::Windowed);
			StateMachine::Get().IsFullscreen = false;
		}
		else
		{
			window.create(sf::VideoMode({ 1600, 900 }), "Game", sf::Style::None, sf::State::Fullscreen);
			StateMachine::Get().IsFullscreen = true;
		}
	}

	if (changeState == true)
	{
		StateMachine::Get().ChangeState(new GameState());
	}
}

void MenuState::MainMenuScreen()
{
	ImGui::Begin("Title_Image", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	ImVec2 WinSize = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2(ScreenSize[0]/2 - WinSize.x/2, 110));
	ImGui::Image(UI_Sprite[SpriteList::UI_Title]);
	ImGui::End();

	//Start
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2), "Select Level", [this]
		{
			current_screen = SelectLevel;
		});

	//Settings
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 100), "Options", [this]
		{
			current_screen = Options;
		});

	//profile
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 200), "Profile", [this]
		{
			current_screen = Profile;
		});

	//Exit
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "Exit", []
		{
			exit(0);
		});

	int VersionTextOffset = 25;
	ImGui::SetNextWindowPos(ImVec2(0,ScreenSize[1] - VersionTextOffset));
	ImGui::Begin("Version", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("Version 1.5.0");
	ImGui::End();
}

void MenuState::SelectLevelScreen()
{
	//Start
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	ImVec2 ScrollbarSize = { ButtonSize.x + 26, ButtonSize.y * 10 };
	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - ScrollbarSize.x / 2, ScreenSize[1] / 2 - ScrollbarSize.y / 2));
	ImGui::SetNextWindowSize(ScrollbarSize);
	ImGui::Begin("##ScrollableBackground", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	LevelButtonUI("Tutorial1", 1, StateMachine::Get().GetLevelStatus(0), 0);

	ImGui::Text("");

	LevelButtonUI("Tutorial2", StateMachine::Get().GetLevelStatus(0), StateMachine::Get().GetLevelStatus(1), 1);

	ImGui::Text("");

	LevelButtonUI("Tutorial3", StateMachine::Get().GetLevelStatus(1), StateMachine::Get().GetLevelStatus(2), 2);
	
	for (int i = 0; i < StateMachine::Get().LevelNumber; i++)
	{
		ImGui::Text("");

		int index = i + 3;
		int lvlnumber = i + 1;
		std::string name = "Level" + std::to_string(lvlnumber);

		LevelButtonUI(name, StateMachine::Get().GetLevelStatus(2), StateMachine::Get().GetLevelStatus(index), index);
	}

	ImGui::End();
	ImGui::PopStyleColor(1);

	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "Back", [this]
		{
			current_screen = MainMenu;
		});
}

void MenuState::OptionsScreen()
{
	//Settings
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2), "Settings", [this]
		{
			current_screen = Settings;
		});


	//Credits
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 150), "Credits", [this]
		{
			current_screen = Credits;
		});


	//Main menu
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "Back", [this]
		{
			current_screen = MainMenu;
		});
}

void MenuState::SettingsScreen(sf::RenderWindow& window)
{
	if (StateMachine::Get().IsFullscreen)
	{
		ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2), "Window Mode", [this]
			{
				changeMode = true;
			});
	}
	else
	{
		ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2), "Fullscreen", [this]
			{
				changeMode = true;
			});
	}

	StateMachine::Get().VolumeSetting(ImVec2(ScreenSize[0]/2 - ButtonSize.x /2 - 8, ScreenSize[1] / 2 - ButtonSize.y / 2 + 100), "Sounds", 0);

	StateMachine::Get().VolumeSetting(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2 - 8, ScreenSize[1] / 2 - ButtonSize.y / 2 + 200), "Music", 1);

	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "Back", [this]
		{
			current_screen = Options;
		});
}

void MenuState::CreditsScreen()
{
	InfoWidget("");
	
	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2 - 55, ScreenSize[1] / 2 - ButtonSize.y / 2 - 30));
	ImGui::SetNextWindowSize(ImVec2(400, 200));
	ImGui::Begin("Links", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |ImGuiWindowFlags_NoDecoration);
	ImGui::Text("Game Creator: Miki18");
	ImGui::Text("");
	ImGui::Text("Font: Textcraft");
	ImGui::Text("");
	ImGui::Text("Music and Sounds:");
	ImGui::Text("Sound FX - BUBBLE POP SOUND EFFECT - FREE");
	ImGui::Text("SOUNDFX FREE - Cute Pop Sound Effects");
	ImGui::Text("Sound Effects - CINEMATIC BOOM");
	ImGui::Text("Kevin MacLeod - Carefree");
	ImGui::End();

	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "Back", [this]
		{
			current_screen = Options;
		});
}

void MenuState::ProfileScreen()
{
	//Credits
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2), "Profile Stats", [this]
		{
			current_screen = ProfileStats;
		});

	//Change profile
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 100), "Change Profile", [this]
		{
			std::ifstream file("Resources/Content/profiles");
			file >> profiles;
			file.close();

			current_screen = SelectProfile;
		});

	//profile
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 200), "Delete Profile", [this]
		{
			current_screen = DeleteProfile;
		});

	//MainMenu
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "Back", [this]
		{
			current_screen = MainMenu;
		});
}

void MenuState::ProfileStatsScreen()
{
	InfoWidget("");

	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2 + 30, ScreenSize[1] / 2 - ButtonSize.y / 2 - 80));
	ImGui::SetNextWindowSize(ImVec2(400, 200));
	ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration);
	ImGui::Text("Zombie: ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(StateMachine::Get().GetPlayerStat(0)).c_str());
	ImGui::Text("Worm: ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(StateMachine::Get().GetPlayerStat(1)).c_str());
	ImGui::Text("Golem: ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(StateMachine::Get().GetPlayerStat(2)).c_str());
	ImGui::Text("Fast Zombie: ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(StateMachine::Get().GetPlayerStat(3)).c_str());
	ImGui::Text("Ghost: ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(StateMachine::Get().GetPlayerStat(4)).c_str());
	ImGui::Text("Umi-bozu: ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(StateMachine::Get().GetPlayerStat(5)).c_str());
	ImGui::Text("Shadow: ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(StateMachine::Get().GetPlayerStat(6)).c_str());
	ImGui::End();

	//Profile
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "Back", [this]
		{
			current_screen = Profile;
		});
}

void MenuState::DeleteProfileScreen()
{
	InfoWidget("Are You Sure?");

	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 200), "Yes", [this]
		{
			for (int i = 0; i < 5; i++)
			{
				if (profiles["profiles"][i]["name"].get<std::string>() == StateMachine::Get().getSelectedProfile())
				{
					profiles["profiles"][i] = nlohmann::json::object();
					std::ofstream file("Resources/Content/profiles");
					file << profiles.dump(2);
					file.close();
					current_screen = SelectProfile;
					break;
				}
			}
		});

	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "No", [this]
		{
			current_screen = Profile;
		});
}

void MenuState::SelectProfileScreen()
{
	if (wrong_nick == true)
	{
		InfoWidget("Incorrect Nick!");

		ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, 700), "Back", [this] {
			new_profile_name[0] = '\0';
			wrong_nick = false;
			});

		return;
	}

	//Load profiles and check if there is a space to add a new profile
	bool IsSlotEmpty = LoadProfilesUI();

	if (IsSlotEmpty == true) { NewProfileUI(); }

	//Exit
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "Exit", []
		{
			exit(0);
		});
}

bool MenuState::LoadProfilesUI()
{
	bool EmptySlot = false;

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2, 0.1, 0.1, 1.0));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4);

	ImGui::SetNextWindowSize(ImVec2(LoadProfilesSize.x, LoadProfilesSize.y));
	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - LoadProfilesSize.x / 2, ScreenSize[1] / 2 - LoadProfilesSize.y / 2 + 100));
	ImGui::Begin("Select Profile", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
	ImVec2 Pos = ImGui::GetCursorScreenPos();

	Pos.y += 10;

	ImVec2 TextSize = ImGui::CalcTextSize("Load Profile");
	ImGui::SetCursorScreenPos(ImVec2(Pos.x + LoadProfilesSize.x / 2 - TextSize.x / 2, Pos.y));
	ImGui::Text("Load Profile");

	Pos.x += 10;
	Pos.y += 25;

	for (int i = 0; i < 5; i++)
	{
		ImGui::SetCursorScreenPos(ImVec2(Pos.x, Pos.y));

		if (profiles["profiles"][i].empty())
		{
			ImGui::Text("Empty");
			EmptySlot = true;
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4, 0.290, 0.190, 1.0));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4, 0.290, 0.190, 1.0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.240, 0.140, 1.0));

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4);

			std::string read_nick = profiles["profiles"][i]["name"];
			if (ImGui::Button(read_nick.c_str()))
			{
				StateMachine::Get().setSelectedProfile(read_nick);
				StateMachine::Get().PassLevel(0, profiles["profiles"][i]["Tutorial1"].get<bool>());
				StateMachine::Get().PassLevel(1, profiles["profiles"][i]["Tutorial2"].get<bool>());
				StateMachine::Get().PassLevel(2, profiles["profiles"][i]["Tutorial3"].get<bool>());
				for (int j = 0; j < StateMachine::Get().LevelNumber; j++)
				{
					std::string Level = "Level" + std::to_string(j + 1);
					StateMachine::Get().PassLevel(j+3, profiles["profiles"][i][Level].get<bool>());
				}
				StateMachine::Get().PassPlayerStat(0, profiles["profiles"][i]["Stats"]["zombie"].get<int>());
				StateMachine::Get().PassPlayerStat(1, profiles["profiles"][i]["Stats"]["worm"].get<int>());
				StateMachine::Get().PassPlayerStat(2, profiles["profiles"][i]["Stats"]["golem"].get<int>());
				StateMachine::Get().PassPlayerStat(3, profiles["profiles"][i]["Stats"]["zombiefast"].get<int>());
				StateMachine::Get().PassPlayerStat(4, profiles["profiles"][i]["Stats"]["ghost"].get<int>());
				StateMachine::Get().PassPlayerStat(5, profiles["profiles"][i]["Stats"]["umibozu"].get<int>());
				StateMachine::Get().PassPlayerStat(6, profiles["profiles"][i]["Stats"]["shadow"].get<int>());

				current_screen = MainMenu;
			}

			ImGui::PopStyleVar(1);
			ImGui::PopStyleColor(3);
		}
		Pos.y += 30;
	}
	ImGui::End();

	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(2);

	return EmptySlot;
}

void MenuState::NewProfileUI()
{
	ImGui::SetNextWindowSize(ImVec2(NewProfileSize.x, NewProfileSize.y));
	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - NewProfileSize.x / 2, ScreenSize[1] / 2 - NewProfileSize.y / 2 - 200));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2, 0.1, 0.1, 1.0));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4);

	ImGui::Begin("NewProfile", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
	ImVec2 Pos = ImGui::GetCursorScreenPos();

	ImVec2 TextSize = ImGui::CalcTextSize("New Profile");
	ImGui::SetCursorScreenPos(ImVec2(Pos.x + NewProfileSize.x / 2 - TextSize.x / 2, Pos.y + 25));
	ImGui::Text("New Profile");

	ImGui::SetCursorScreenPos(ImVec2(Pos.x + 25, Pos.y + 50));
	ImGui::PushItemWidth(140);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0.5));
	ImGui::InputText("##Input text", new_profile_name, 15);
	ImGui::PopStyleColor(1);
	ImGui::PopItemWidth();

	TextSize = ImGui::CalcTextSize("Done");
	ImGui::SetCursorScreenPos(ImVec2(Pos.x + NewProfileSize.x / 2 - TextSize.x / 2 - 7, Pos.y + 100));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.240, 0.140, 1.0));

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4);
	if (ImGui::Button("Done"))
	{
		bool VerifyNick = true;

		//Only numbers and ASCII letters
		for (int i = 0; i < strlen(new_profile_name); i++)
		{
			if (!std::isalnum(static_cast<unsigned char>(new_profile_name[i])))
			{
				VerifyNick = false;
				break;
			}
		}

		if (std::string(new_profile_name).length() == 0)
		{
			VerifyNick = false;
		}

		if (VerifyNick == true)
		{
			//nick needs to be unique
			for (int i = 0; i < 5; i++)
			{
				if (!profiles["profiles"][i].empty())
				{
					if (profiles["profiles"][i]["name"] == new_profile_name)
					{
						VerifyNick = false;
						break;
					}
				}
			}
		}

		if (VerifyNick == false)
		{
			wrong_nick = true;
		}
		else
		{
			for (int i = 0; i < 5; i++)
			{
				if (profiles["profiles"][i].empty())
				{
					profiles["profiles"][i]["name"] = new_profile_name;
					profiles["profiles"][i]["Tutorial1"] = false;
					profiles["profiles"][i]["Tutorial2"] = false;
					profiles["profiles"][i]["Tutorial3"] = false;
					for (int j = 0; j < StateMachine::Get().LevelNumber; j++)
					{
						std::string Level = "Level" + std::to_string(j + 1);
						profiles["profiles"][i][Level] = false;
					}
					profiles["profiles"][i]["Stats"] = {
						{"zombie", 0},
						{"worm", 0},
						{"golem", 0},
						{"zombiefast", 0},
						{"ghost", 0},
						{"umibozu", 0},
						{"shadow", 0}
					};
					std::ofstream file("Resources/Content/profiles");
					file << profiles.dump(2);
					file.close();
					break;
				}
			}
		}
		new_profile_name[0] = '\0';
	}

	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(3);
	ImGui::End();

	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(2);
}

void MenuState::ButtonUI(ImVec2 Pos, std::string name, std::function<void()> OnClick)
{
	ImGui::SetNextWindowPos(ImVec2(Pos.x - 8, Pos.y));

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.240, 0.140, 1.0));

	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2, 0.1, 0.1, 1.0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4);

	ImGui::Begin((name + "button").c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
	if (ImGui::Button(name.c_str(), ImVec2(ButtonSize.x, ButtonSize.y)))
	{
		OnClick();
	}

	ImGui::End();

	ImGui::PopStyleVar(1);

	ImGui::PopStyleColor(4);
}

void MenuState::LevelButtonUI(std::string name, bool IsActive, bool IsColored, int level)
{
	if (IsActive == false)
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5, 0.475, 0.440, 1.0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.475, 0.440, 1.0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5, 0.475, 0.440, 1.0));
	}
	else if (IsColored == true)
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0.65, 0.0, 1.0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65, 0.65, 0.0, 1.0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0.45, 0.0, 1.0));
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4, 0.290, 0.190, 1.0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4, 0.290, 0.190, 1.0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.240, 0.140, 1.0));
	}

	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2, 0.1, 0.1, 1.0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4);

	if (ImGui::Button(name.c_str(), ImVec2(ButtonSize.x, ButtonSize.y)))
	{
		if (IsActive)
		{
			StateMachine::Get().SelectedLevel = level;
			changeState = true;
		}
	}

	ImGui::PopStyleVar(1);

	ImGui::PopStyleColor(4);
}

void MenuState::InfoWidget(std::string message)
{
	sf::Vector2f picSize = { 310,210 };

	ImGui::SetNextWindowSize(ImVec2(picSize.x, picSize.y + 5));
	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2, 0.1, 0.1, 1.0));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4);

	ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
	ImVec2 name_size = ImGui::CalcTextSize(message.c_str());
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 textPos = ImVec2(
		windowPos.x + picSize.x / 2 - name_size.x / 2,
		windowPos.y + picSize.y / 2 - name_size.y / 2
	);
	ImGui::SetCursorScreenPos(textPos);
	ImGui::Text(message.c_str());
	ImGui::End();

	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(2);
}
