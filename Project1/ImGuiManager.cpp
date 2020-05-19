#include "ImGuiManager.h"
#include"imgui/imgui.h"

//コンストラクタ
ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

//デストラクタ
ImGuiManager::~ImGuiManager()
{
	ImGui::DestroyContext();
}
