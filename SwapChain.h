#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>

class CommandQueue;

class SwapChain
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SwapChain(ID3D12CommandQueue* commandQueue, IDXGIFactory7* dxgiFactory);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SwapChain();

	/// <summary>
	/// スワップチェーンの取得
	/// </summary>
	/// <returns></returns>
	IDXGISwapChain4* GetSwapChain() {
		return swapChain_.Get();
	}

	/// <summary>
	/// バックバッファ数の取得
	/// </summary>
	/// <returns></returns>
	UINT GetBackBuffer() {
		return swapChain_->GetCurrentBackBufferIndex();
	}
private:
	/// <summary>
	/// ファクトリーの生成
	/// </summary>
	/// <returns></returns>
	HRESULT CreateFactory();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	/// <returns></returns>
	HRESULT CreateSwap();

private:
	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;

	// ファクトリー
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;

	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;

	// バックバッファ数
	UINT backBufferCount;

};

