#ifndef _APP_H_
#define _APP_H_

#include"Window.h"
#include"Timer.h"
#include"ImGuiManager.h"
#include"Camera.h"
#include"PointLight.h"
#include<set>

class App
{
public:
	App();
	//���b�Z�[�W���[�v
	int Go();
	~App();
private:
	void DoFrame();
	void SpawnSimulationWindow()noexcept;
	void SpawnBoxWindowManagerWindow()noexcept;
	void SpawnBoxWindows()noexcept;
private:
	ImGuiManager imgui;
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	std::vector<class Box*> boxes;
	float speed_factor = 1.0f;
	static constexpr size_t nDrawables = 300;
	Camera camera;
	//�^���I�ɑ��z��`�悷��
	PointLight light;

	//�l�������A�܂��͑��݂��Ȃ���
	std::optional<int> comboBoxIndex;
	//�l���̂��L�[�ƂȂ�
	std::set<int>boxControlIds;
};
#endif
