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
