#include "WindowsAPI.h"
#include <string>

#ifdef _DEBUG
#include "externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT
ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // _DEBUG

namespace {
	// SJIS -> WideChar
	std::wstring ConvertString(const std::string& str) {
		if (str.empty()) {
			return std::wstring();
		}

		auto size_needed =
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), NULL, 0);
		if (size_needed == 0) {
			return std::wstring();
		}
		std::wstring result(size_needed, 0);
		MultiByteToWideChar(
			CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), &result[0], size_needed);
		return result;
	}

	// ref: https://devblogs.microsoft.com/oldnewthing/20131017-00/?p=2903
	BOOL UnadjustWindowRectEx(LPRECT prc, DWORD dwStyle, BOOL fMenu, DWORD dwExStyle) {
		RECT rc;
		SetRectEmpty(&rc);
		BOOL fRc = AdjustWindowRectEx(&rc, dwStyle, fMenu, dwExStyle);
		if (fRc) {
			prc->left -= rc.left;
			prc->top -= rc.top;
			prc->right -= rc.right;
			prc->bottom -= rc.bottom;
		}
		return fRc;
	}
} // namespace

WindowsAPI* WindowsAPI::GetInstance()
{
	static WindowsAPI instance;
	return &instance;
}

LRESULT WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}


	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WindowsAPI::CreateGameWindow(const char* title, UINT windowStyle, int32_t clientWidth, int32_t clientHeight)
{
	// COM初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	//ウィンドウプロシージャ
	wndClass_.lpfnWndProc = WindowProc;
	//ウィンドウクラス名
	wndClass_.lpszClassName = L"CG2WindowClass";
	//インスタンスハンドル
	wndClass_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wndClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wndClass_);

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,clientWidth,clientHeight };

	//クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウタイトルをwchar_tに変換
	std::wstring titleWString = ConvertString(title);

	hwnd_ = CreateWindow(
		wndClass_.lpszClassName, // 利用するクラス名
		titleWString.c_str(),	 // タイトルバーの文字(なんでもよい)
		windowStyle,	         // よく見るウィンドウスタイル
		CW_USEDEFAULT,			 // 表示X座標(Windowsに任せる)
		CW_USEDEFAULT,			 // 表示Y座標(WindowsOSに任せる)
		wrc.right - wrc.left,	 // ウィンドウ横幅
		wrc.bottom - wrc.top,	 // ウィンドウ縦幅
		nullptr,				 // 親ウィンドウハンドル
		nullptr,				 // メニューハンドル
		wndClass_.hInstance,	 // インスタンスハンドル
		nullptr);				 // オプション

	// ウィンドウ表示
	ShowWindow(hwnd_, SW_NORMAL);
}

void WindowsAPI::TerminateGameWindow()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(wndClass_.lpszClassName, wndClass_.hInstance);

	// COM 終了
	CoUninitialize();
}

bool WindowsAPI::ProcessMessage()
{
	MSG msg{};// メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg);  // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}
