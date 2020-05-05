#include "Window.h"
#include<sstream>
#include"resource.h"

//Window Class Stuff
Window::WindowClass Window::WindowClass::wndClass;

//�R���X�g���N�^
//GetModuleHandle�ŃC���X�^���X���擾���邱�Ƃ��ł���
Window::WindowClass::WindowClass()noexcept
	:hInstance(GetModuleHandle(nullptr)) {

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);    //�\���̃T�C�Y
	wc.style = CS_HREDRAW | CS_VREDRAW;//�E�B���h�E�̃X�^�C��
	wc.lpfnWndProc = HandleMsgSetup;          //�E�B���h�E�̃��b�Z�[�W����������R�[���o�b�N�֐�
	wc.cbClsExtra = 0;                 //�E�B���h�E�N���X�\���̂̌��Ɋ��蓖�Ă�⑫�o�C�g��
	wc.cbWndExtra = 0;                 //�E�B���h�E�C���X�^���X�̌��Ɋ��蓖�Ă�⑫�o�C�g��
	wc.hInstance = GetInstance();      //�E�B���h�E�v���V�[�W���̃C���X�^���X�n���h��
	wc.hCursor = nullptr;              //�}�E�X�J�[�\���̃n���h��(LoadCursor)
	wc.hIcon = LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_ICON2));//�A�C�R���̃n���h��(WindowApp���Ƃ��Q��)
	wc.hbrBackground = nullptr;        //�E�B���h�E�̔w�i�F
	wc.lpszMenuName = nullptr;         //�f�t�H���g���j���[��
	wc.lpszClassName = GetName();      //�E�B���h�E�N���X��
	wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON2));              //16�~16�̏������T�C�Y�̃A�C�R��
	//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
	//�E�B���h�E�N���X�̓o�^����
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName()noexcept {
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance()noexcept {
	return wndClass.hInstance;
}

//Window Stuff
Window::Window(int width, int height, const char* name) {
	//calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	//�킴�ƃG���[��\������
	//�����O����
	//throw WIN_EXCEPT(ERROR_ARENA_TRASHED);
	//�W����O����
	//throw std::runtime_error("butts butts buuuuuuuuuuuttsssssssss");
	//Unknown Exception
	//throw 123456789;

	// �E�B���h�E�̍쐬 & HWND�̎擾
	hWnd = CreateWindow(
		WindowClass::GetName(),
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		nullptr,
		nullptr,
		WindowClass::GetInstance(),
		this
		);
	//�E�B���h�E������ɍ��ꂽ���ǂ���
	if (hWnd == nullptr) {
		throw WIN_LAST_EXCEPT();
	}

	//�E�B���h�E�̕\��
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window() {
	//�E�B���h�E�̔j��
	DestroyWindow(hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//CreateWindow()����n�����쐬�p�����[�^���g�p���ăE�B���h�E�N���X�̃|�C���^���i�[����
	if (msg == WM_NCCREATE) {
		//�v���V�[�W���ɓn����鏉�����p�����[�^���`����
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		//WinAPI���Ǘ����郆�[�U�[�f�[�^��ݒ肵�āA�E�B���h�E�N���X�ɕۑ�����
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		//�v���V�[�W����ʏ�̃n���h���ɐݒ肷��
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		//���b�Z�[�W��]������
		return pWnd->WndProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	//forward message to window class handler
	return pWnd->WndProc(hWnd, msg, wParam, lParam);
}

//�v���V�[�W��
LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept {

	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
		//�L�[�{�[�h�t�H�[�J�X���������O�ɌĂ΂��
	case WM_KILLFOCUS:
		keyboard.ClearState();
		break;
		//�L�[�{�[�h����
	case WM_KEYDOWN:
		//Alt�L�[�Ȃǂ̃V�X�e���L�[��ǐՂ������Ȃ�WM_SYSKEYDOWN���w�肷��
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || keyboard.AutoRepeatIsEnabled())
			keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
		//�L�[�{�[�h���� �I��
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//��O�����N���X
//�R���X�g���N�^
//�p�����WinException�̃R���X�g���N�^��������(C#��base�I��)
Window::Exception::Exception(int line, const char* file, HRESULT hr)noexcept
	:WinException(line, file), hr(hr) {

}

const char* Window::Exception::what()const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType()const noexcept {
	return "Window Exception";
}

//�G���[���b�Z�[�W��Ԃ�
std::string Window::Exception::TranslateErrorCode(HRESULT hr)noexcept {
	char* pMsgBuf = nullptr;
	//FormatMessage = �G���[�R�[�h�̐������擾
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
	if (nMsgLen == 0) {
		return "Unidentified error code";
	}
	std::string errorString = pMsgBuf;
	//�w�肳�ꂽ���[�J��������������A�n���h���𖳌��ɂ���
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept {
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept {
	return TranslateErrorCode(hr);
}