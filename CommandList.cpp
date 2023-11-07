#include "CommandList.h"
#include <cassert>
CommandList::CommandList(ID3D12Device* device)
{
	device_ = device;
	CreateAllocator();
	CreateList();
}

CommandList::~CommandList()
{
}

HRESULT CommandList::CreateAllocator()
{
	HRESULT result = S_FALSE;
	result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result));
	return result;
}

HRESULT CommandList::CreateList()
{
	HRESULT result = S_FALSE;
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(result));

	// コマンドリストを一時クローズ
	//commandList_->Close();
	return result;
}

