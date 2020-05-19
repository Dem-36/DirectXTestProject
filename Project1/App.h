#ifndef _APP_H_
#define _APP_H_

#include"Window.h"
#include"Timer.h"

class App
{
public:
	App();
	//メッセージループ
	int Go();
	~App();
private:
	void DoFrame();
private:
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 300;
};
#endif
