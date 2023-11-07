#include "Fence.h"
#include <cassert>

Fence::Fence(ID3D12Device* device, ID3D12CommandQueue* commandQueue)
{
	assert(device);
	device_ = device;
	commandQueue_ = commandQueue;
	CreateFence();
}

Fence::~Fence()
{
}

void Fence::waitForCommandsToFinish()
{
	// コマンドリストの実行完了を待つ
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}

HRESULT Fence::CreateFence()
{
	HRESULT result = S_FALSE;
	// フェンスの生成
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(result));
	return result;
}
