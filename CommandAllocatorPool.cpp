#include "CommandAllocatorPool.h"
#include <cassert>
#pragma comment(lib, "d3d12.lib")

CommandAllocatorPool::CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type) : commandListType(type), device_(nullptr)
{
}

CommandAllocatorPool::~CommandAllocatorPool()
{
	Release();
}

void CommandAllocatorPool::Create(ID3D12Device* device)
{
	device_ = device;
}

void CommandAllocatorPool::Release()
{
	for (size_t i = 0; i < allocatorPool_.size(); ++i) {
		allocatorPool_[i]->Release();
	}
	allocatorPool_.clear();
}

ID3D12CommandAllocator* CommandAllocatorPool::RequestAllocator(uint64_t completedFenceValue)
{
	std::lock_guard<std::mutex> lockGuard(allocatorMutex_);

	ID3D12CommandAllocator* allocator = nullptr;
	if (readyAllocators_.empty()) {
		std::pair<uint64_t, ID3D12CommandAllocator*>& allocatorPair = readyAllocators_.front();

		if (allocatorPair.first <= completedFenceValue) {
			allocator = allocatorPair.second;
			assert(SUCCEEDED(allocator->Reset()));
			readyAllocators_.pop();
		}
	}

	if (allocator == nullptr) {
		assert(SUCCEEDED(device_->CreateCommandAllocator(commandListType, IID_PPV_ARGS(&allocator))));
		wchar_t allocatorName[32];
		swprintf(allocatorName, 32, L"CommandAllocator %zu", allocatorPool_.size());
		allocator->SetName(allocatorName);
		allocatorPool_.push_back(allocator);
	}

	return allocator;
}

void CommandAllocatorPool::DiscardAllocator(uint64_t fenceValue, ID3D12CommandAllocator* allocator)
{
	std::lock_guard<std::mutex> lockGuard(allocatorMutex_);
	readyAllocators_.push(std::make_pair(fenceValue, allocator));


}
