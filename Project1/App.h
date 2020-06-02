#ifndef _APP_H_
#define _APP_H_

#include"Window.h"
#include"Utility\Timer.h"
#include"ImGuiManager.h"
#include"Camera.h"
#include"PointLight.h"
#include<set>
#include"Mesh.h"

class App
{
public:
	App();
	//���b�Z�[�W���[�v
	int Go();
	~App();
private:
	void DoFrame();
	void ShowModelWindow();
private:
	ImGuiManager imgui;
	Window wnd;
	Timer timer;
	float speed_factor = 1.0f;
	Camera camera;
	//�^���I�ɑ��z��`�悷��
	PointLight light;
	Model nano{ wnd.Gfx(),"Models\\nanosuit.obj" };

	struct {
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	}position;

};
#endif
