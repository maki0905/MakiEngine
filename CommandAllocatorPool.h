#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <stdint.h>
#include <d3d12.h>
#include <wrl.h>

class CommandAllocatorPool
{
public :
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">タイプ</param>
	CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE  type);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CommandAllocatorPool();

	/// <summary>
	/// コマンドアロケータプールの生成
	/// </summary>
	/// <param name="device">デバイス</param>
	void Create(ID3D12Device* device);

	/// <summary>
	/// リリース
	/// </summary>
	void Release();

	/// <summary>
	/// リクエスト
	/// </summary>
	/// <param name="completedFenceValue"></param>
	/// <returns></returns>
	ID3D12CommandAllocator* RequestAllocator(uint64_t completedFenceValue);

	/// <summary>
	/// コマンドアロケータの破棄
	/// </summary>
	/// <param name="fenceValue"></param>
	/// <param name="allocator"></param>
	void DiscardAllocator(uint64_t fenceValue, ID3D12CommandAllocator* allocator);

	/// <summary>
	/// プール内のアロケータの数を取得
	/// </summary>
	/// <returns></returns>
	inline size_t Size() { return allocatorPool_.size(); }

private:
	// コマンドリストのタイプ
	const D3D12_COMMAND_LIST_TYPE commandListType;

	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;

	// コマンドアロケータのプール
	std::vector<ID3D12CommandAllocator*> allocatorPool_;
	
	// 使用可能なアロケータのキュー
	std::queue<std::pair<uint64_t, ID3D12CommandAllocator*>> readyAllocators_;

	// ミューテックス
	std::mutex allocatorMutex_;
};

