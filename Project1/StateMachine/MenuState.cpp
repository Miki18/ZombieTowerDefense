#include "MenuState.h"
#include "StateMachine.h"

#include <cctype>

MenuState::MenuState()
{
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

void MenuState::Input(sf::RenderWindow& window, sf::Clock& DeltaClock)
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

void MenuState::Update(sf::Clock& DeltaClock)
{
	switch (current_screen)
	{
	case MenuState::SelectProfile:
		SelectProfileScreen();
		break;
	case MenuState::MainMenu:
		MainMenuScreen();
		break;
	default:
		break;
	}
}

void MenuState::Render(sf::RenderWindow& window)
{
	window.clear(sf::Color::Blue);

	ImGui::SFML::Render(window);
	window.display();
}

void MenuState::MainMenuScreen()
{
	//Start
	ButtonUI(ImVec2(ScreenSize[0] / 2 - 125 / 2, ScreenSize[1] / 2 - 100 / 2 - 100), ImVec2(250, 100), "Start", [] {
		StateMachine::Get().ChangeState(new GameState());
		});


	//Back
	ButtonUI(ImVec2(ScreenSize[0] / 2 - 125 / 2, ScreenSize[1] / 2 - 100 / 2 + 100), ImVec2(250, 100), "Back", [this] {

			std::ifstream file("Resources/Content/profiles.json");
			file >> profiles;
			file.close();

			current_screen = SelectProfile;
		});


	//Exit
	ButtonUI(ImVec2(ScreenSize[0] / 2 - 125 / 2, ScreenSize[1] / 2 - 100 / 2 + 300), ImVec2(250, 100), "Exit", [] {
		exit(0);
		});
}

void MenuState::SelectProfileScreen()
{
	//Load profiles and check if there is a space to add a new profile
	bool IsSlotEmpty = LoadProfilesUI();
	
	if (IsSlotEmpty == true) { NewProfileUI(); }
}

bool MenuState::LoadProfilesUI()
{
	bool EmptySlot = false;

	ImGui::SetNextWindowPos(ImVec2(ScreenSize[0] / 2, ScreenSize[1] / 2));
	ImGui::Begin("Select Profile");

	for (int i = 0; i < 5; i++)
	{
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
	}
	ImGui::End();

	return EmptySlot;
}

void MenuState::NewProfileUI()
{
	ImGui::Begin("NewProfile");
	ImGui::InputText("Input text", new_profile_name, 15);
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
			//Info player
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
	ImGui::End();
}

void MenuState::ButtonUI(ImVec2 Pos, ImVec2 Size, std::string name, std::function<void()> OnClick)
{
	ImGui::SetNextWindowPos(Pos);
	ImGui::Begin((name + "button").c_str(), nullptr, ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoBackground);

	if (ImGui::Button(name.c_str(), Size))
	{
		OnClick();
	}
	ImGui::End();
}