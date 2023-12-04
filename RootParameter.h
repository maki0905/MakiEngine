#pragma once

#include <d3d12.h>
class RootParameter
{
public:
	RootParameter();
	~RootParameter();

	void Clear();

	void InitializeAsConstants(UINT shaderRegister, UINT numDwords, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, UINT registerSpace = 0);
	void InitializeAsConstantBuffer(UINT shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, UINT registerSpace = 0);
	void InitializeAsBufferSRV(UINT shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, UINT registerSpace = 0);
	void InitializeAsBufferUAV(UINT shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, UINT registerSpace = 0);
	void InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shaderRegister, UINT count, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, UINT registerSpace = 0);
	void InitializeAsDescriptorTable(UINT rangeCount, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetTableRange(UINT rangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shaderRegister, UINT count, UINT registerSpace = 0);

	const D3D12_ROOT_PARAMETER& operator() (void) const { return rootParameter_; }

private:
	D3D12_ROOT_PARAMETER rootParameter_;

};

