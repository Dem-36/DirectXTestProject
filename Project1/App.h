#ifndef _APP_H_
#define _APP_H_

#include"Window.h"
#include"Timer.h"

class App
{
public:
	App();
	//���b�Z�[�W���[�v
	int Go();
private:
	void DoFrame();
private:
	Window wnd;
	Timer timer;
};
#endif
