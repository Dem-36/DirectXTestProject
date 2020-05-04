#include<Windows.h>
#include"WindowsMessageMap.h"

//�E�B���h�E�v���V�[�W�� ���b�Z�[�W�������s��
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//Main
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	const char* pClassName = "hw3dbutts";
	//register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);    //�\���̃T�C�Y
	wc.style = CS_HREDRAW | CS_VREDRAW;//�E�B���h�E�̃X�^�C��
	wc.lpfnWndProc = WndProc;          //�E�B���h�E�̃��b�Z�[�W����������R�[���o�b�N�֐�
	wc.cbClsExtra = 0;                 //�E�B���h�E�N���X�\���̂̌��Ɋ��蓖�Ă�⑫�o�C�g��
	wc.cbWndExtra = 0;                 //�E�B���h�E�C���X�^���X�̌��Ɋ��蓖�Ă�⑫�o�C�g��
	wc.hInstance = hInstance;          //�E�B���h�E�v���V�[�W���̃C���X�^���X�n���h��
	wc.hCursor = nullptr;              //�}�E�X�J�[�\���̃n���h��(LoadCursor)
	wc.hIcon = nullptr;                //�A�C�R���̃n���h��(WindowApp���Ƃ��Q��)
	wc.hbrBackground = nullptr;        //�E�B���h�E�̔w�i�F
	wc.lpszMenuName = nullptr;         //�f�t�H���g���j���[��
	wc.lpszClassName = pClassName;     //�E�B���h�E�N���X��
	wc.hIconSm = nullptr;              //16�~16�̏������T�C�Y�̃A�C�R��
	RegisterClassEx(&wc);

	//Create window instance
	HWND hWnd = CreateWindowEx(
		0,                                       //�g���X�^�C���̎w��
		pClassName,                              //�E�B���h�E�N���X��
		"Happy Hard Window",                     //�^�C�g��
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200,                                     //X���W
		200,                                     //Y���W
		640,                                     //��
		480,                                     //����
		nullptr,                                 //�e�E�B���h�E�̃n���h��
		nullptr, 
		hInstance,                               //�E�B���h�E�Ɋ֘A�t����ꂽ�C���X�^���X�n���h��
		nullptr);                                

	//show the damn window
	ShowWindow(hWnd, nCmdShow);

	//message pump
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		//�L�[���̓��b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�����
		TranslateMessage(&msg);
		//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		DispatchMessage(&msg);
	}

	return 0;
}

//�E�B���h�E�v���V�[�W��
//wParam,lParam�ɂ̓L�[�{�[�h�̃R�[�h�A�}�E�X�J�[�\���̍��W�Ȃǂ��i�[����Ă���
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	//�o�͂ɑ����Ă��郁�b�Z�[�W����\������
	static WindowsMessageMap mm;
	OutputDebugString(mm(msg, lParam, wParam).c_str());

	switch (msg) {
		//�E�B���h�E������ꂽ�Ƃ�
	case WM_CLOSE:
		//WM_QUIT�𑗂�
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		//wParam�ɉ��z�L�[�R�[�h��񂪓���
		if (wParam == 'F') {
			SetWindowText(hWnd, "Respects");
		}
		break;
	case WM_KEYUP:
		if (wParam == 'F') {
			SetWindowText(hWnd, "Dangerfield");
		}
		break;
		//�e�L�X�g�Ȃǂőł����߂�L�[�������ꂽ�Ƃ�
	case WM_CHAR:
		//static std::string title;
		//title.push_back((char)wParam);
		//SetWindowText(hWnd, title.c_str());
		//break;
		//�E�B���h�E���Ń}�E�X�̍��N���b�N�������ꂽ�Ƃ�
	case WM_LBUTTONDOWN:
		//lParam�̉���16�r�b�g��x���W,���16�r�b�g��y���W�������Ă���
		//int x = LOWORD(lParam);
		//int y = HIWORD(lParam);
		//lParam�̏�񂩂�x,y�̏����擾
		POINTS pt = MAKEPOINTS(lParam);
		char szBuf[256];
		wsprintf(szBuf, TEXT("X %d : Y %d"), pt.x, pt.y);
		SetWindowText(hWnd, szBuf);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}