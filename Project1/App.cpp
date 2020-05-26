#include "App.h"
#include"Box.h"
#include"Melon.h"
#include"Pyramid.h"
#include"WinMath.h"
#include"Cylinder.h"
#include"Sheet.h"
#include<algorithm>
#include<memory>
#include"SkinnedBox.h"

#include"imgui/imgui.h"
#include"AssTest.h"

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
					odist, rdist, bdist, mat
					);
			case 1:
				return std::make_unique<Cylinder>(
					gfx, rng, adist, ddist, odist, rdist, bdist, tdist
					);
			case 2:
				return std::make_unique<Pyramid>(
					gfx, rng, adist, ddist, odist, rdist, tdist
					);
			case 3:
				return std::make_unique<SkinnedBox>(
					gfx, rng, adist, ddist, odist, rdist
					);
			case 4:
				return std::make_unique<AssTest>(
					gfx, rng, adist, ddist, odist, rdist,mat,1.5f
					);
			default:
				assert(false && "impossible drawable option in factory");
				return {};

			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_int_distribution<int> sdist{ 0,4 };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f }; //0 ~ 90
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
		std::uniform_int_distribution<int> tdist{ 3,30 };
	};

	//�v�f�����w��
	drawables.reserve(nDrawables);
	//�w�肵���ʒu�ȍ~�̊e�v�f�ɁA�w��̊֐���֐��I�u�W�F�N�g�ɂ���Đ������ꂽ�l����
	std::generate_n(std::back_inserter(drawables), nDrawables, Factory{ wnd.Gfx() });

	for (auto& pd : drawables)
	{
		if (auto pb = dynamic_cast<Box*>(pd.get()))
		{
			boxes.push_back(pb);
		}
	}

	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	wnd.Gfx().SetCamera(camera.GetMatrix());
}

int App::Go()
{
	while (true) {
		//�ۗ����̂��ׂẴ��b�Z�[�W���������܂����A
		//�V�������b�Z�[�W���u���b�N���܂���
		if (const auto ecode = Window::ProcessMessage()) {
			//optional�ɒl������ꍇ�͏I�����Ă���̂ŏI���R�[�h��Ԃ�
			wnd.Release();
			return *ecode;
		}
		DoFrame();
	}
}

App::~App() {}

//�X�V�������s��
void App::DoFrame()
{
	auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(camera.GetMatrix());
	//�����Œ萔�s�N�Z���o�b�t�@���o�C���h���Ă���̂�
	//Box�Ƀ��C�g�̈ʒu���n����Ă���H
	// = ���ɒ萔�o�b�t�@���o�C���h�����܂ł̓��C�g�̒萔�o�b�t�@���n�����
	light.Bind(wnd.Gfx(), wnd.Gfx().GetCamera());
	for (auto& b : drawables) {
		b->Update(dt);
		b->Draw(wnd.Gfx());
	}
	light.Draw(wnd.Gfx());

	SpawnSimulationWindow();
	camera.SpawnControlWindow();
	light.SpawnControlWindow();

	SpawnBoxWindowManagerWindow();
	SpawnBoxWindows();
	//present
	wnd.Gfx().EndFrame();
}

//���x���Ǘ�����E�B���h�E���쐬
void App::SpawnSimulationWindow() noexcept
{
	if (ImGui::Begin("Simulation Speed")) {
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 6.0f, "%.4f", 3.2F);
		ImGui::Text("%.3F MS/FRAME (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.GetKeyboard()->KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
	}
	ImGui::End();
}

//Box��ID���w�肵�Ă��̏���ύX�ł���E�B���h�E�����(Manager)
void App::SpawnBoxWindowManagerWindow() noexcept
{
	if (ImGui::Begin("Boxes")) {
		using namespace std::string_literals;
		const auto preview = comboBoxIndex ? std::to_string(*comboBoxIndex) : "Choose a box..."s;
		//�v���_�E���𐶐�
		if (ImGui::BeginCombo("Box Nimber", preview.c_str())) {
			for (int i = 0; i < boxes.size(); i++) {
				const bool selected = *comboBoxIndex == i;
				if (ImGui::Selectable(std::to_string(i).c_str(), selected)) {
					comboBoxIndex = i;
				}
				if (selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Spawn Control Window") && comboBoxIndex) {
			boxControlIds.insert(*comboBoxIndex);
			comboBoxIndex.reset();
		}
	}
	ImGui::End();
}

//Box�̏���ύX�ł���E�B���h�E
void App::SpawnBoxWindows() noexcept
{
	for (auto i = boxControlIds.begin(); i != boxControlIds.end();) {
		if (!boxes[*i]->SpawnControlWindow(*i, wnd.Gfx())) {
			i = boxControlIds.erase(i);
		}
		else {
			i++;
		}
	}
}
