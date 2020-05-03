#include<Windows.h>

//ウィンドウプロシージャ メッセージ処理を行う
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
	wc.cbSize = sizeof(WNDCLASSEX);    //構造のサイズ
	wc.style = CS_HREDRAW | CS_VREDRAW;//ウィンドウのスタイル
	wc.lpfnWndProc = WndProc;          //ウィンドウのメッセージ処理をするコールバック関数
	wc.cbClsExtra = 0;                 //ウィンドウクラス構造体の後ろに割り当てる補足バイト数
	wc.cbWndExtra = 0;                 //ウィンドウインスタンスの後ろに割り当てる補足バイト数
	wc.hInstance = hInstance;          //ウィンドウプロシージャのインスタンスハンドル
	wc.hCursor = nullptr;              //マウスカーソルのハンドル(LoadCursor)
	wc.hIcon = nullptr;                //アイコンのハンドル(WindowApp作るとき参照)
	wc.hbrBackground = nullptr;        //ウィンドウの背景色
	wc.lpszMenuName = nullptr;         //デフォルトメニュー名
	wc.lpszClassName = pClassName;     //ウィンドウクラス名
	wc.hIconSm = nullptr;              //16×16の小さいサイズのアイコン
	RegisterClassEx(&wc);

	//Create window instance
	HWND hWnd = CreateWindowEx(
		0,                                       //拡張スタイルの指定
		pClassName,                              //ウィンドウクラス名
		"Happy Hard Window",                     //タイトル
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200,                                     //X座標
		200,                                     //Y座標
		640,                                     //幅
		480,                                     //高さ
		nullptr,                                 //親ウィンドウのハンドル
		nullptr, 
		hInstance,                               //ウィンドウに関連付けられたインスタンスハンドル
		nullptr);                                

	//show the damn window
	ShowWindow(hWnd, nCmdShow);

	//message pump
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		//キー入力メッセージを文字メッセージに変換する
		TranslateMessage(&msg);
		//プロシージャにメッセージを送る
		DispatchMessage(&msg);
	}

	return 0;
}

//ウィンドウプロシージャ
//wParam,lParamにはキーボードのコード、マウスカーソルの座標などが格納されている
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
		//ウィンドウが閉じられたとき
	case WM_CLOSE:
		//WM_QUITを送る
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}