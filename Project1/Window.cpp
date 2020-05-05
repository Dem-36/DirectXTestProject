#include "Window.h"
#include<sstream>
#include"resource.h"

//Window Class Stuff
Window::WindowClass Window::WindowClass::wndClass;

//コンストラクタ
//GetModuleHandleでインスタンスを取得することができる
Window::WindowClass::WindowClass()noexcept
	:hInstance(GetModuleHandle(nullptr)) {

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);    //構造のサイズ
	wc.style = CS_HREDRAW | CS_VREDRAW;//ウィンドウのスタイル
	wc.lpfnWndProc = HandleMsgSetup;          //ウィンドウのメッセージ処理をするコールバック関数
	wc.cbClsExtra = 0;                 //ウィンドウクラス構造体の後ろに割り当てる補足バイト数
	wc.cbWndExtra = 0;                 //ウィンドウインスタンスの後ろに割り当てる補足バイト数
	wc.hInstance = GetInstance();      //ウィンドウプロシージャのインスタンスハンドル
	wc.hCursor = nullptr;              //マウスカーソルのハンドル(LoadCursor)
	wc.hIcon = LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_ICON2));//アイコンのハンドル(WindowApp作るとき参照)
	wc.hbrBackground = nullptr;        //ウィンドウの背景色
	wc.lpszMenuName = nullptr;         //デフォルトメニュー名
	wc.lpszClassName = GetName();      //ウィンドウクラス名
	wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON2));              //16×16の小さいサイズのアイコン
	//ウィンドウクラスの登録
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
	//ウィンドウクラスの登録解除
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

	//わざとエラーを表示する
	//自作例外処理
	//throw WIN_EXCEPT(ERROR_ARENA_TRASHED);
	//標準例外処理
	//throw std::runtime_error("butts butts buuuuuuuuuuuttsssssssss");
	//Unknown Exception
	//throw 123456789;

	// ウィンドウの作成 & HWNDの取得
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
	//ウィンドウが正常に作られたかどうか
	if (hWnd == nullptr) {
		throw WIN_LAST_EXCEPT();
	}

	//ウィンドウの表示
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window() {
	//ウィンドウの破棄
	DestroyWindow(hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//CreateWindow()から渡される作成パラメータを使用してウィンドウクラスのポインタを格納する
	if (msg == WM_NCCREATE) {
		//プロシージャに渡される初期化パラメータを定義する
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		//WinAPIが管理するユーザーデータを設定して、ウィンドウクラスに保存する
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		//プロシージャを通常のハンドラに設定する
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		//メッセージを転送する
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

//プロシージャ
LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept {

	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
		//キーボードフォーカスを失う直前に呼ばれる
	case WM_KILLFOCUS:
		keyboard.ClearState();
		break;
		//キーボード処理
	case WM_KEYDOWN:
		//Altキーなどのシステムキーを追跡したいならWM_SYSKEYDOWNを指定する
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
		//キーボード処理 終了
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//例外処理クラス
//コンストラクタ
//継承先のWinExceptionのコンストラクタも初期化(C#のbase的な)
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

//エラーメッセージを返す
std::string Window::Exception::TranslateErrorCode(HRESULT hr)noexcept {
	char* pMsgBuf = nullptr;
	//FormatMessage = エラーコードの説明を取得
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
	if (nMsgLen == 0) {
		return "Unidentified error code";
	}
	std::string errorString = pMsgBuf;
	//指定されたローカルメモリを解放、ハンドルを無効にする
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept {
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept {
	return TranslateErrorCode(hr);
}