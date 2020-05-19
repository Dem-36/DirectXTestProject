#include "ImGuiManager.h"
#include"imgui/imgui.h"

//�R���X�g���N�^
ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

//�f�X�g���N�^
ImGuiManager::~ImGuiManager()
{
	ImGui::DestroyContext();
}
