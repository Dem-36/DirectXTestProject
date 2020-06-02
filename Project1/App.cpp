#include "App.h"
#include"Utility/WinMath.h"
#include<algorithm>
#include<memory>

#include"imgui/imgui.h"

namespace dx = DirectX;

App::App()
	:wnd(800, 600, "Geometry Parade"),
	light(wnd.Gfx())
{
	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

int App::Go()
{
	while (true) {
		//保留中のすべてのメッセージを処理しますが、
		//新しいメッセージをブロックしません
		if (const auto ecode = Window::ProcessMessage()) {
			//optionalに値がある場合は終了しているので終了コードを返す
			wnd.Release();
			return *ecode;
		}
		DoFrame();
	}
}

App::~App() {}

//更新処理を行う
void App::DoFrame()
{
	auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(camera.GetMatrix());
	//ここで定数ピクセルバッファをバインドしているので
	//Boxにライトの位置が渡されている？
	// = 次に定数バッファがバインドされるまではライトの定数バッファが渡される
	light.Bind(wnd.Gfx(), wnd.Gfx().GetCamera());

	const auto transform = dx::XMMatrixRotationRollPitchYaw(position.roll, position.pitch, position.yaw) * 
		dx::XMMatrixTranslation(position.x,position.y,position.z);
	nano.Draw(wnd.Gfx(), transform);

	light.Draw(wnd.Gfx());

	camera.SpawnControlWindow();
	light.SpawnControlWindow();
	ShowModelWindow();
	//present
	wnd.Gfx().EndFrame();
}

void App::ShowModelWindow()
{
	if (ImGui::Begin("Model")) {
		using namespace std::string_literals;

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &position.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &position.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &position.yaw, -180.0f, 180.0f);

		ImGui::Text("Position");
		ImGui::SliderFloat("X", &position.x, -20.0f, 20.0f);
		ImGui::SliderFloat("Y", &position.y, -20.0f, 20.0f);
		ImGui::SliderFloat("Z", &position.z, -20.0f, 20.0f);
	}
	ImGui::End();
}
