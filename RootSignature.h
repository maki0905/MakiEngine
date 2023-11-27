#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <vector>
#include <wrl.h>
#include <memory>

class RootParameter {
public:
	RootParameter();
	~RootParameter();
	void Clear();

	void InitalizeAsConstants(UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL, UINT space = 0);
	void InitalizeAsConstantBuffer(UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL, UINT space = 0);
	void InitalizeAsBufferSRV(UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL, UINT space = 0);
	void InitalizeAsBufferUAV(UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL, UINT space = 0);
	void InitalizeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT Register, UINT count, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL, UINT space = 0);
	void InitalizeAsDescriptorTable(UINT RangeCount, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetTableRange(UINT rangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT Register, UINT count, UINT space = 0);
	const D3D12_ROOT_PARAMETER& operator() (void) const { return rootParameter_; }
protected:
	D3D12_ROOT_PARAMETER rootParameter_;
};

class RootSignature {
public:
	RootSignature(ID3D12Device* device, UINT numRootParams = 0, UINT numStaticSamplers = 0);
	~RootSignature();
	void Reset(UINT numRootParams, UINT numStaticSamplers = 0);
	void InitalizeStaticSampler(UINT Register, const D3D12_SAMPLER_DESC& nonStaticSamplerDesc, D3D12_SHADER_VISIBILITY visibility);
	void Finalize(D3D12_ROOT_SIGNATURE_FLAGS Flags);

protected:
	UINT numParameters_;
	UINT numSamplers_;
	UINT numInitializedStaticSamplers_;
	uint32_t descriptorTableBitMap_;
	uint32_t samplerTableBitMap_;
	uint32_t descriptorTableSize_[16];
	std::unique_ptr<RootParameter[]> parameterArray_;
	std::unique_ptr <D3D12_STATIC_SAMPLER_DESC[]> samplerArray_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
};