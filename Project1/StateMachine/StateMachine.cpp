#include "StateMachine.h"

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

StateMachine& StateMachine::Get()
{
	static StateMachine Instance;
	return Instance;
}

void StateMachine::ChangeState(StateManager* newState)
{
	delete State;
	State = newState;
}

void StateMachine::Run(sf::RenderWindow& window)
{
	deltaClock.start();
	srand(time(NULL));

	while (window.isOpen())
	{
		sf::Time time = deltaClock.restart();

		State->Input(window, time);
		State->Update(window, time);
		State->Render(window);
	}
}

std::string StateMachine::getSelectedProfile()
{
	return selected_profile;
}

void StateMachine::setSelectedProfile(std::string nick)
{
	selected_profile = nick;
}

void StateMachine::PassLevel(int index, bool Check)
{
	Tutorials[index] = Check;
}

bool StateMachine::GetLevelStatus(int index)
{
	return Tutorials[index];
}

void StateMachine::VolumeSetting(ImVec2 Pos, std::string name, bool IsMusic)
{
	ImGui::SetNextWindowPos(Pos);

	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.4, 0.290, 0.190, 1.0));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.35, 0.240, 0.140, 1.0));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.35, 0.240, 0.140, 1.0));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.35, 0.240, 0.140, 1.0));

	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2, 0.1, 0.1, 1.0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4);

	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

	ImVec2 TxtSize = ImGui::CalcTextSize(name.c_str());

	ImGui::SetCursorPos(ImVec2(ImGui::GetItemRectSize()[0]/2 - TxtSize[0]/2, 0));

	ImGui::Text(name.c_str());

	name = "##" + name;

	if (IsMusic)
	{
		ImGui::SliderFloat(name.c_str(), &MusicVolume, 0.0f, 100.0f);
	}
	else
	{
		ImGui::SliderFloat(name.c_str(), &SoundVolume, 0.0f, 100.0f);
	}

	ImGui::End();

	ImGui::PopStyleVar(1);

	ImGui::PopStyleColor(6);
}

float StateMachine::GetVolume(bool isMusic)
{
	if (isMusic)
	{
		return MusicVolume;
	}
	else
	{
		return SoundVolume;
	}
}
