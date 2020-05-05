#ifndef _WINDOW_H_
#define _WINDOW_H_

#include"WinInc.h"
#include"WinException.h"
#include"Keyboard.h"

class Window
{
public:
	class Exception :public WinException {
	public:
		Exception(int line, const char* file, HRESULT hr)noexcept;
		const char* what()const noexcept override;
		virtual const char* GetType()const noexcept;
		static std::string TranslateErrorCode(HRESULT hr)noexcept;
		HRESULT GetErrorCode()const noexcept;
		std::string GetErrorString()const noexcept;
	private:
		HRESULT hr;
	};
private:
	//�E�B���h�E�쐬�N���X
	//noexcept = ���̊֐�����O�����𓊂��Ȃ����Ƃ�ۏ؂���
	class WindowClass {
	public:
		//����O�����w��
		static const char* GetName()noexcept;
		static HINSTANCE GetInstance()noexcept;
	private:
		WindowClass()noexcept;
		~WindowClass();
		//delete�錾 = ����֐��̈Öْ�`�𖾎��I�ɋ֎~����
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator = (const WindowClass&) = delete;
		//constexpr�������֐��̓R���p�C�����Ɍv�Z���邱�Ƃ��ł���ƕ\��
		static constexpr const char* wndClassName = "DirectX Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInstance;
	};
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	//�v���V�[�W��
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;
public:
	Keyboard keyboard;
private:
	int width;
	int height;
	HWND hWnd;
};

//�v���v���Z�b�T��__LINE__���s�ԍ��̐����ɕϊ�����
//�v���v���Z�b�T��__FILE__��PGM�̃t�@�C����
#define WIN_EXCEPT(hr) Window::Exception(__LINE__,__FILE__,hr)
//GetLastError() = �Ăяo�����̃X���b�h�����ŐV�̃G���[�R�[�h���擾
#define WIN_LAST_EXCEPT() Window::Exception(__LINE__,__FILE__,GetLastError())

#endif

