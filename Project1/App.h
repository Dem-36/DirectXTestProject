#ifndef _APP_H_
#define _APP_H_

#include"Window.h"
#include"Timer.h"
#include"ImGuiManager.h"
#include"Camera.h"
#include"PointLight.h"

class App
{
public:
	App();
	//���b�Z�[�W���[�v
	int Go();
	~App();
private:
	void DoFrame();
private:
	ImGuiManager imgui;
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	float speed_factor = 1.0f;
	static constexpr size_t nDrawables = 180;
	Camera camera;
	//�^���I�ɑ��z��`�悷��
	PointLight light;
};
#endif
