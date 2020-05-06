#include "Window.h"
#include<sstream>
#include"resource.h"

//Window Class Stuff ここでWindowClass のコンストラクタが呼ばれる
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
Window::Window(int width, int height, const char* name)
	:width(width), height(height) {
	//calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) {
		throw WIN_LAST_EXCEPT();
	}

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
	keyboard = new Keyboard();
	mouse = new Mouse;

	//create graphics object
	pGfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window() {
	//ウィンドウの破棄
	DestroyWindow(hWnd);
}

//タイトルが正常にセットされない場合は例外処理を流す
void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0) {
		throw WIN_LAST_EXCEPT();
	}
}

//解放処理
void Window::Release()
{
	delete keyboard;
	delete mouse;
}

//外部でキーボード判定を取得
Keyboard* Window::GetKeyboard()
{
	return keyboard;
}

Mouse* Window::GetMouse()
{
	return mouse;
}

std::optional<int> Window::ProcessMessage()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return (int)msg.wParam;
		}
		//キー入力メッセージを文字メッセージに変換する
		TranslateMessage(&msg);
		//プロシージャにメッセージを送る
		DispatchMessage(&msg);
	}
	return {};
}

Graphics& Window::Gfx()
{
	return *pGfx;
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

#pragma region Keyboard

		//キーボードフォーカスを失う直前に呼ばれる
	case WM_KILLFOCUS:
		keyboard->ClearState();
		break;
		//キーボード処理
	case WM_KEYDOWN:
		//Altキーなどのシステムキーを追跡したいならWM_SYSKEYDOWNを指定する
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || keyboard->AutoRepeatIsEnabled())
			keyboard->OnKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		keyboard->OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		keyboard->OnChar(static_cast<unsigned char>(wParam));
		break;
		//キーボード処理 終了

#pragma endregion Keyboard

#pragma region Mouse

	case WM_MOUSEMOVE: {
		//lParamにマウス座標が記録されている
		//MAKEPOINTSでPOINTS型の位置が取得できる
		POINTS points = MAKEPOINTS(lParam);
		//マウスがウィンドウ内にいるなら
		if (points.x >= 0 && points.x < width &&
			points.y >= 0 && points.y < height) {

			mouse->OnMouseMove(points.x, points.y);
			//マウスが外から入ってきたら = それまでウィンドウ内に存在しなかった
			if (!mouse->IsInWindow()) {
				//ボタンが離されるまでウィンドウの外にマウスが移動しても
				//そのウィンドウがマウスを制御することができる
				SetCapture(hWnd);
				mouse->OnMouseEnter();
			}
		}
		else {
			//左又は右クリックが押されているなら
			//https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mousemove
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
				mouse->OnMouseMove(points.x, points.y);
			}
			else {
				//マウスのキャプチャーを解放
				ReleaseCapture();
				mouse->OnMouseLeave();
			}
		}
	}
	case WM_LBUTTONDOWN: {
		POINTS points = MAKEPOINTS(lParam);
		mouse->OnLeftPressed(points.x, points.y);
		break;
	}
	case WM_RBUTTONDOWN: {
		POINTS points = MAKEPOINTS(lParam);
		mouse->OnRightPressed(points.x, points.y);
		break;
	}
	case WM_LBUTTONUP: {
		POINTS points = MAKEPOINTS(lParam);
		mouse->OnLeftReleased(points.x, points.y);
		break;
	}
	case WM_RBUTTONUP: {
		POINTS points = MAKEPOINTS(lParam);
		mouse->OnRightReleased(points.x, points.y);
		break;
	}
	case WM_MOUSEWHEEL: {
		POINTS points = MAKEPOINTS(lParam);
		int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse->OnWheelDelta(points.x, points.y, delta);
		//wParamからホイールデルタ値を取得する
		//数値が+なら前に、-なら後ろに動かしている
		//0の場合は静止状態
		//if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
		//	mouse->OnWheelUp(points.x, points.y);
		//}
		//else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
		//	mouse->OnWheelDown(points.x, points.y);
		//}
		break;
	}
#pragma endregion Mouse

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}