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
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().EndFrame();
}
