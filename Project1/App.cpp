#include "App.h"

App::App()
	:wnd(800, 600, "The Donkey Fart Box")
{
}

int App::Go()
{
	while (true) {
		//保留中のすべてのメッセージを処理しますが、
		//新しいメッセージをブロックしません
		if (const auto ecode = Window::ProcessMessage()) {
			//optionalに値がある場合は終了しているので終了コードを返す
			return *ecode;
		}
		DoFrame();
	}
}

//更新処理を行う
void App::DoFrame()
{
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().EndFrame();
}
