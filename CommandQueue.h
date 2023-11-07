#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>


class CommandQueue
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CommandQueue(ID3D12Device* device);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CommandQueue();

	/// <summary>
	/// コマンドキューの取得
	/// </summary>
	/// <returns></returns>
	ID3D12CommandQueue* GetCommandQueue() {
		return commandQueue_.Get();
	}

private:
	/// <summary>
	///  コマンドキューの生成
	/// </summary>
	/// <returns></returns>
	HRESULT CreateQueue();

	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;

	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;

};

