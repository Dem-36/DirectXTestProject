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
	//メッセージループ
	int Go();
	~App();
private:
	void DoFrame();
	void ShowImGuiDemoWindow();
private:
	ImGuiManager imgui;
	Window wnd;
	Timer timer;
	float speed_factor = 1.0f;
	Camera camera;
	//疑似的に太陽を描画する
	PointLight light;
	Model nano{ wnd.Gfx(),"Models\\nanosuit.obj" };
};
#endif
