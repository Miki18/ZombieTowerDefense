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

	if (StateMachine::Get().getSelectedProfile().length() == 0)
	{
		std::ifstream file("Resources/Content/profiles.json");
		file >> profiles;
		file.close();

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

void MenuState::Update(sf::Time time)
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

		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

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
		SettingsScreen();
		break;
	case MenuState::Credits:
		CreditsScreen();
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

	if (changeState == true)
	{
		StateMachine::Get().ChangeState(new GameState());
	}

}

void MenuState::MainMenuScreen()
{
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

	//Change profile
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 200), "Change Profile", [this]
		{
			std::ifstream file("Resources/Content/profiles.json");
			file >> profiles;
			file.close();

			current_screen = SelectProfile;
		});

	//Exit
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "Exit", []
		{
			exit(0);
		});
}

void MenuState::SelectLevelScreen()
{
	//Start
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2), "Tutorial 1", [this]
		{
			//TODO mark, that player finished this level
			//StateMachine::Get().ChangeState(new GameState());
			changeState = true;
		});

	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "Back", [this]
		{
			current_screen = MainMenu;
		});
}

void MenuState::OptionsScreen()
{
	//settings
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 - 100), "Settings", [this]
		{
			current_screen = Settings;
		});


	//Back
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 100), "Credits", [this]
		{
			current_screen = Credits;
		});


	//Exit
	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - 100 / 2 + 300), "Back", [this]
		{
			current_screen = MainMenu;
		});
}

void MenuState::SettingsScreen()
{
	sf::Vector2f picSize = { 310,210 };

	ImGui::SetNextWindowSize(ImVec2(picSize.x, picSize.y + 5));
	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2));

	ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::Image(UI_Sprite[SpriteList(UI_BigRectangle)]);
	ImDrawList* drawlist = ImGui::GetWindowDrawList();
	ImVec2 name_size = ImGui::CalcTextSize("There's nothing to do here yet.");
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 textPos = ImVec2(
		windowPos.x + picSize.x / 2 - name_size.x / 2,
		windowPos.y + picSize.y / 2 - name_size.y / 2
	);
	drawlist->AddText(textPos, IM_COL32_WHITE, "There's nothing to do here yet.");
	ImGui::End();

	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "Back", [this]
		{
			current_screen = Options;
		});
}

void MenuState::CreditsScreen()
{
	sf::Vector2f picSize = { 310,210 };

	ImGui::SetNextWindowSize(ImVec2(picSize.x, picSize.y + 5));
	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2));

	ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::Image(UI_Sprite[SpriteList(UI_BigRectangle)]);
	ImDrawList* drawlist = ImGui::GetWindowDrawList();
	ImVec2 name_size = ImGui::CalcTextSize("Game made by Miki18");
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 textPos = ImVec2(
		windowPos.x + picSize.x / 2 - name_size.x / 2,
		windowPos.y + picSize.y / 2 - name_size.y / 2
	);
	drawlist->AddText(textPos, IM_COL32_WHITE, "Game made by Miki18");
	ImGui::End();

	ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, ScreenSize[1] / 2 - ButtonSize.y / 2 + 300), "Back", [this]
		{
			current_screen = Options;
		});
}

void MenuState::SelectProfileScreen()
{
	if (wrong_nick == true)
	{
		sf::Vector2f picSize = { 310,210 };

		ImGui::SetNextWindowSize(ImVec2(picSize.x, picSize.y + 5));
		ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - picSize.x / 2, ScreenSize[1] / 2 - picSize.y / 2));

		ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::Image(UI_Sprite[SpriteList(UI_BigRectangle)]);
		ImDrawList* drawlist = ImGui::GetWindowDrawList();
		ImVec2 name_size = ImGui::CalcTextSize("Incorrect nick!");
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 textPos = ImVec2(
			windowPos.x + picSize.x / 2 - name_size.x / 2,
			windowPos.y + picSize.y / 2 - name_size.y / 2
		);
		drawlist->AddText(textPos, IM_COL32_WHITE, "Incorrect nick!");
		ImGui::End();

		ButtonUI(ImVec2(ScreenSize[0] / 2 - ButtonSize.x / 2, 700), "Back", [this] {
			new_profile_name[0] = '\0';
			wrong_nick = false;
			});

		return;
	}

	//Load profiles and check if there is a space to add a new profile
	bool IsSlotEmpty = LoadProfilesUI();

	if (IsSlotEmpty == true) { NewProfileUI(); }
}

bool MenuState::LoadProfilesUI()
{
	bool EmptySlot = false;

	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - LoadProfilesSize.x / 2, ScreenSize[1] / 2 - LoadProfilesSize.y / 2 + 100));
	ImGui::Begin("Select Profile", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
	ImVec2 Pos = ImGui::GetCursorScreenPos();
	ImGui::Image(UI_Sprite[SpriteList(UI_LoadProfiles)]);

	Pos.y += 10;

	ImVec2 TextSize = ImGui::CalcTextSize("Load Profile");
	ImGui::SetCursorScreenPos(ImVec2(Pos.x + LoadProfilesSize.x / 2 - TextSize.x / 2, Pos.y));
	ImGui::Text("Load Profile");

	Pos.x += 10;
	Pos.y += 25;

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.5));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0.5));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0.75));

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
			std::string read_nick = profiles["profiles"][i]["name"];
			if (ImGui::Button(read_nick.c_str()))
			{
				StateMachine::Get().setSelectedProfile(read_nick);
				current_screen = MainMenu;
			}
		}
		Pos.y += 30;
	}

	ImGui::PopStyleColor(3);
	ImGui::End();

	return EmptySlot;
}

void MenuState::NewProfileUI()
{
	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2 - NewProfileSize.x / 2, ScreenSize[1] / 2 - NewProfileSize.y / 2 - 200));

	ImGui::Begin("NewProfile", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
	ImVec2 Pos = ImGui::GetCursorScreenPos();
	ImGui::Image(UI_Sprite[SpriteList(UI_NewProfile)]);

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
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.5));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0.5));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0.75));
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
					std::ofstream file("Resources/Content/profiles.json");
					file << profiles.dump(2);
					file.close();
					break;
				}
			}
		}
		new_profile_name[0] = '\0';
	}
	ImGui::PopStyleColor(3);
	ImGui::End();
}

void MenuState::ButtonUI(ImVec2 Pos, std::string name, std::function<void()> OnClick)
{
	ImGui::SetNextWindowPos(Pos);

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0.25));

	ImGui::Begin((name + "button").c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
	if (ImGui::ImageButton(name.c_str(), UI_Sprite[SpriteList(UI_RectangleButton)], ButtonSize, sf::Color(255, 255, 255, 255)))
	{
		OnClick();
	}
	ImDrawList* drawlist = ImGui::GetWindowDrawList();
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 cursorPos = ImGui::GetCursorPos();

	ImVec2 buttonPos = ImVec2(windowPos.x + cursorPos.x, windowPos.y + ButtonSize.y / 5);

	ImVec2 name_size = ImGui::CalcTextSize(name.c_str());
	ImVec2 textPos = ImVec2(
		buttonPos.x + ButtonSize.x / 2 - name_size.x / 2,
		buttonPos.y + ButtonSize.y / 2 - name_size.y / 2
	);

	drawlist->AddText(textPos, IM_COL32_WHITE, name.c_str());

	ImGui::End();

	ImGui::PopStyleColor(3);
}