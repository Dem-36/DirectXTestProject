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
	//メッセージループ
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
	//疑似的に太陽を描画する
	PointLight light;

	//値を持つか、または存在しないか
	std::optional<int> comboBoxIndex;
	//値自体がキーとなる
	std::set<int>boxControlIds;
};
#endif
