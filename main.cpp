#include <Windows.h>
#include "DirectXApp.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//WindowsAPI* win = nullptr;
	//// ゲームウィンドウの作成
	//win = WindowsAPI::GetInstance();
	//win->CreateGameWindow();

	//while (true) {
	//	if (win->ProcessMessage()) {
	//		break;
	//	}
	//}

	//// ゲームウィンドウの破棄
	//win->TerminateGameWindow();


	DirectXApp* app = new DirectXApp();
	app->Run();
	return 0;
}