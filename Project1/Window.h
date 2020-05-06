#ifndef _WINDOW_H_
#define _WINDOW_H_

#include"WinInc.h"
#include"WinException.h"
#include"Keyboard.h"
#include"Mouse.h"
#include"Graphics.h"
#include <optional>
#include<memory>

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
	Keyboard* keyboard;
	Mouse* mouse;
	int width;
	int height;
	HWND hWnd;
	//リソースへのポインタ所有権を唯一持っている(コピー不可 ムーブを使うと所有権を譲渡できる)
	std::unique_ptr<Graphics> pGfx;
};

//プリプロセッサが__LINE__を行番号の整数に変換する
//プリプロセッサが__FILE__をPGMのファイル名
#define WIN_EXCEPT(hr) Window::Exception(__LINE__,__FILE__,hr)
//GetLastError() = 呼び出し側のスレッドが持つ最新のエラーコードを取得
#define WIN_LAST_EXCEPT() Window::Exception(__LINE__,__FILE__,GetLastError())

#endif

