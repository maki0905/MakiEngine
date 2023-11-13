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
	
	void BarrierChange(IDXGISwapChain4* swapChain, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	void SetRenderTargets(IDXGISwapChain4* swapChain, D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandels, ID3D12DescriptorHeap* dsvHeap);

	void SetViewport(float width, float height);

	void SetRect(UINT width, UINT height);

	void CommandClear();

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

