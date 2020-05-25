#include "App.h"
#include"Box.h"
#include"Melon.h"
#include"Pyramid.h"
#include"WinMath.h"
#include"Cylinder.h"
#include"Sheet.h"
#include<algorithm>
#include<memory>

#include"imgui/imgui.h"

namespace dx = DirectX;

App::App()
	:wnd(800, 600, "Geometry Parade"),
	light(wnd.Gfx())
{
	class Factory {
	public:
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			const DirectX::XMFLOAT3 mat = { cdist(rng),cdist(rng),cdist(rng) };
			switch (sdist(rng)) {
			case 0:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist,mat
					);
			case 1:
				return std::make_unique<Cylinder>(
					gfx, rng, adist, ddist, odist, rdist,bdist,tdist
					);
			case 2:
				return std::make_unique<Pyramid>(
					gfx, rng, adist, ddist, odist, rdist, tdist
					);
			default:
				assert(false && "impossible drawable option in factory");
				return {};

			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_int_distribution<int> sdist{ 0,2 };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_real_distribution<float> cdist{ 0.0f,1.0f};
		std::uniform_int_distribution<int> tdist{ 3,30 };
	};

	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, Factory{ wnd.Gfx() });

	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	wnd.Gfx().SetCamera(camera.GetMatrix());
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
	light.Bind(wnd.Gfx(),wnd.Gfx().GetCamera());
	for (auto& b : drawables) {
		b->Update(dt);
		b->Draw(wnd.Gfx());
	}
	light.Draw(wnd.Gfx());

	if (ImGui::Begin("Simulation Speed")) {
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.GetKeyboard()->KeyIsPressed(VK_SPACE));
	}
	ImGui::End();
	camera.SpawnControlWindow();
	light.SpawnControlWindow();
	//present
	wnd.Gfx().EndFrame();
}
