#ifndef _WINDOW_H_
#define _WINDOW_H_

#include"WinInc.h"
#include"WinExceptionMacro.h"
#include"Utility/Keyboard.h"
#include"Utility/Mouse.h"
#include"Graphics.h"
#include <optional>
#include<memory>

class Window
{
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
	void SetTitle(const std::string& title);
	void Release();
	Keyboard* GetKeyboard();
	Mouse* GetMouse();
	static std::optional<int> ProcessMessage();
	Graphics& Gfx();
private:
	//�v���V�[�W��
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;
private:
	//���L���͗B��
	std::unique_ptr<Keyboard> keyboard;
	std::unique_ptr<Mouse> mouse;
	int width;
	int height;
	HWND hWnd;
	//���\�[�X�ւ̃|�C���^���L����B�ꎝ���Ă���(�R�s�[�s�� ���[�u���g���Ə��L�������n�ł���)
	std::unique_ptr<Graphics> pGfx;
};
#endif

