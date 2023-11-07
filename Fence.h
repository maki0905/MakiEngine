#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <vector>
#include <wrl.h>


class Fence
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">デバイス</param>
	Fence(ID3D12Device* device, ID3D12CommandQueue* commandQueue);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Fence();

	/// <summary>
	/// コマンドリストの実行完了を待つ
	/// </summary>
	void waitForCommandsToFinish();

private:
	/// <summary>
	/// フェンス生成
	/// </summary>
	HRESULT CreateFence();

private:
	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;

	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;

	// フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence>fence_;

	UINT64 fenceVal_ = 0;

};

