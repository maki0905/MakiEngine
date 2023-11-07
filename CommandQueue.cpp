#include "CommandQueue.h"
#include <cassert>
CommandQueue::CommandQueue(ID3D12Device* device)
{
	device_ = device;
	CreateQueue();
}

CommandQueue::~CommandQueue()
{
}

HRESULT CommandQueue::CreateQueue()
{
	HRESULT result = S_FALSE;

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{}; // 標準設定
	result = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result));
	return result;
}
