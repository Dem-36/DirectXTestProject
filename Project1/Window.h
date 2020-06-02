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
	//ウィンドウ作成クラス
	//noexcept = その関数が例外処理を投げないことを保証する
	class WindowClass {
	public:
		//無例外処理指定
		static const char* GetName()noexcept;
		static HINSTANCE GetInstance()noexcept;
	private:
		WindowClass()noexcept;
		~WindowClass();
		//delete宣言 = 特殊関数の暗黙定義を明示的に禁止する
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator = (const WindowClass&) = delete;
		//constexprをつけた関数はコンパイル時に計算することもできると表す
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
	//プロシージャ
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;
private:
	//所有権は唯一
	std::unique_ptr<Keyboard> keyboard;
	std::unique_ptr<Mouse> mouse;
	int width;
	int height;
	HWND hWnd;
	//リソースへのポインタ所有権を唯一持っている(コピー不可 ムーブを使うと所有権を譲渡できる)
	std::unique_ptr<Graphics> pGfx;
};
#endif

