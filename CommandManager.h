#pragma once

#include <memory>
#include "CommandList.h"
#include "CommandQueue.h"

class CommandManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">デバイス</param>
	CommandManager(ID3D12Device* device);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CommandManager();

	ID3D12GraphicsCommandList* GetCommandList() { return commandList_->GetCommandList(); }
	ID3D12CommandQueue* GetCommandQueue() { return commandQueue_->GetCommandQueue(); }
	/// <summary>
	/// コマンドアロケータの取得
	/// </summary>
	/// <returns></returns>
	ID3D12CommandAllocator* GetCommandAllocator() {
		return commandList_->GetCommandAllocator();
	}
private:

private:
	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	std::unique_ptr<CommandList> commandList_;
	std::unique_ptr<CommandQueue> commandQueue_;
};

