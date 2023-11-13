#include "CommandManager.h"
#include <cassert>
CommandManager::CommandManager(ID3D12Device* device)
{
	assert(device);

	device_ = device;

	commandList_ = std::make_unique<CommandList>(device);

	commandQueue_ = std::make_unique<CommandQueue>(device);
}

CommandManager::~CommandManager()
{
}


void CommandManager::BarrierChange(IDXGISwapChain4* swapChain, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	HRESULT result = S_FALSE;
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	// リソースバリアを変更（表示状態→描画対象）
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	result = swapChain->GetBuffer(bbIndex, IID_PPV_ARGS(&barrier.Transition.pResource));
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	commandList_->GetCommandList()->ResourceBarrier(1, &barrier);
}

void CommandManager::SetRenderTargets(IDXGISwapChain4* swapChain, D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandels, ID3D12DescriptorHeap* dsvHeap)
{
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// レンダーターゲットをセット
	commandList_->GetCommandList()->OMSetRenderTargets(1, &rtvHandels[bbIndex], false, nullptr);
}

void CommandManager::SetViewport(float width, float height)
{
	// ビューポートの設定
	D3D12_VIEWPORT viewport =
		D3D12_VIEWPORT(0.0f, 0.0f, width, height);
	commandList_->GetCommandList()->RSSetViewports(1, &viewport);
}

void CommandManager::SetRect(UINT width, UINT height)
{
	// シザリング矩形の設定
	D3D12_RECT rect = D3D12_RECT(0, 0, width, height);
	commandList_->GetCommandList()->RSSetScissorRects(1, &rect);
}

void CommandManager::CommandClear()
{
	commandList_->GetCommandAllocator()->Reset(); // キューをクリア
	commandList_->GetCommandList()->Reset(commandList_->GetCommandAllocator(),
		nullptr); // 再びコマンドリストを貯める準備

}

