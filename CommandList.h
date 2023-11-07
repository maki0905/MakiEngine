#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>

class CommandList
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CommandList(ID3D12Device* device);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CommandList();

	/// <summary>
	/// コマンドリストの取得
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCommandList() {
		return commandList_.Get();
	}

	/// <summary>
	/// コマンドアロケータの取得
	/// </summary>
	/// <returns></returns>
	ID3D12CommandAllocator* GetCommandAllocator() {
		return commandAllocator_.Get();
	}

private:
	/// <summary>
	/// コマンドアロケータの生成
	/// </summary>
	/// <returns></returns>
	HRESULT CreateAllocator();

	/// <summary>
	/// コマンドリストの生成
	/// </summary>
	/// <returns></returns>
	HRESULT CreateList();

	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;

	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;

	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
};

