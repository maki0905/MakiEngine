﻿#pragma once
#include <Windows.h>
#include <cstdint>

/// <summary>
/// ウィンドウズアプリケーション
/// </summary>
class WindowsAPI
{
public: // 静的メンバ変数
	// ウィンドウサイズ
	static const int kWindowWidth = 1280; // 横幅
	static const int kWindowHeight = 720; // 縦幅
public: // 静的メンバ関数
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static WindowsAPI* GetInstance();

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージ番号</param>
	/// <param name="wparam">メッセージ情報1</param>
	/// <param name="lparam">メッセージ情報2</param>
	/// <returns>成否</returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // メンバ関数
	/// <summary>
	/// ゲームウィンドウの作成
	/// <param name="title">ウィンドウタイトル</param>
	/// <param name="windowStyle">ウィンドウの初期スタイル</param>
	/// <param name="clientWidth">ウィンドウのクライアント領域の初期幅</param>
	/// <param name="clientHeight">ウィンドウのクライアント領域の初期高さ</param>
	/// </summary>
	void CreateGameWindow(
		const char* title = "MakiEngine", UINT windowStyle = WS_OVERLAPPEDWINDOW,
		int32_t clientWidth = kWindowWidth, int32_t clientHeight = kWindowHeight);

	/// <summary>
	/// ゲームウィンドウの破棄
	/// </summary>
	void TerminateGameWindow();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns>終了かどうか</returns>
	bool ProcessMessage();

	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() const { return hwnd_; }

	HINSTANCE GetHInstance() const { return wndClass_.hInstance; }

private: // メンバ関数
	WindowsAPI() = default;
	~WindowsAPI() = default;
	WindowsAPI(const WindowsAPI&) = delete;
	const WindowsAPI& operator=(const WindowsAPI&) = delete;

private: // メンバ変数
	// Window関連
	HWND hwnd_ = nullptr;   // ウィンドウハンドル
	WNDCLASS wndClass_{}; // ウィンドウクラス
};

