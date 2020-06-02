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

	nano.Draw(wnd.Gfx());
	light.Draw(wnd.Gfx());

	camera.SpawnControlWindow();
	light.SpawnControlWindow();
	ShowImGuiDemoWindow();
	nano.ShowWindow();
	//present
	wnd.Gfx().EndFrame();
}

void App::ShowImGuiDemoWindow()
{
	static bool show_demo_window = true;
	if (show_demo_window) {
		ImGui::ShowDemoWindow(&show_demo_window);
	}
}
