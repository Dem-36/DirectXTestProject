#include "App.h"

App::App()
	:wnd(800, 600, "The Donkey Fart Box")
{
}

int App::Go()
{
	while (true) {
		//�ۗ����̂��ׂẴ��b�Z�[�W���������܂����A
		//�V�������b�Z�[�W���u���b�N���܂���
		if (const auto ecode = Window::ProcessMessage()) {
			//optional�ɒl������ꍇ�͏I�����Ă���̂ŏI���R�[�h��Ԃ�
			return *ecode;
		}
		DoFrame();
	}
}

//�X�V�������s��
void App::DoFrame()
{
	const float c = (float)sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	//Peek = �ۑ����Ă���ŏ��̎��Ԃ��猻�ݎ��Ԃ�����(Mark�ōX�V)
	wnd.Gfx().DrawTriangle(-timer.Peek(),
		0,
		0);
	wnd.Gfx().DrawTriangle(timer.Peek(),
		wnd.GetMouse()->GetX() / 400.0f - 1.0f,
		-wnd.GetMouse()->GetY() / 300.0f + 1.0f);

	wnd.Gfx().EndFrame();
}
