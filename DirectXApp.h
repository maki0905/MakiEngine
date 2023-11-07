#pragma once
#include <memory>

#include "CommandManager.h"
#include "Depth.h"
#include "Device.h"
#include "Fence.h"
#include "RenderTarget.h"
#include "SwapChain.h"
#include "WindowsAPI.h"

class DirectXApp
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DirectXApp();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DirectXApp();

	void Run();


private:
	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();


private:
	WindowsAPI* win_ = nullptr;
	Device* device_ = nullptr;
	CommandManager* commandManager_ = nullptr;
	SwapChain* swapChain_ = nullptr;
	RenderTarget* renderTarget_ = nullptr;
	Depth* depth_ = nullptr;
	Fence* fence_ = nullptr;


};

