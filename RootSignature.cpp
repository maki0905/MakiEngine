#include "RootSignature.h"
#include "externals/DirectXTex/DirectXTex.h"
#include <d3dcompiler.h>
#include <dxcapi.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "externals/imgui/imgui.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <map>
#include <mutex>



RootParameter::RootParameter()
{
	rootParameter_.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
}

RootParameter::~RootParameter()
{
	Clear();
}

void RootParameter::Clear()
{
	if (rootParameter_.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE) {
		delete[] &rootParameter_.DescriptorTable.NumDescriptorRanges;
	}

	rootParameter_.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
}

void RootParameter::InitalizeAsConstants(UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY visibility, UINT space)
{
	rootParameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	rootParameter_.ShaderVisibility = visibility;
	rootParameter_.Constants.Num32BitValues = NumDwords;
	rootParameter_.Constants.ShaderRegister = Register;
	rootParameter_.Constants.RegisterSpace = space;
}

void RootParameter::InitalizeAsConstantBuffer(UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY visibility, UINT space)
{
	rootParameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter_.ShaderVisibility = visibility;
	rootParameter_.Descriptor.ShaderRegister = Register;
	rootParameter_.Descriptor.RegisterSpace = space;


}

void RootParameter::InitalizeAsBufferSRV(UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY visibility, UINT space)
{
	rootParameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	rootParameter_.ShaderVisibility = visibility;
	rootParameter_.Descriptor.ShaderRegister = Register;
	rootParameter_.Descriptor.RegisterSpace = space;
}

void RootParameter::InitalizeAsBufferUAV(UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY visibility, UINT space)
{
	rootParameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameter_.ShaderVisibility = visibility;
	rootParameter_.Descriptor.ShaderRegister = Register;
	rootParameter_.Descriptor.RegisterSpace = space;
}

void RootParameter::InitalizeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT Register, UINT count, D3D12_SHADER_VISIBILITY visibility, UINT space)
{
	InitalizeAsDescriptorTable(1, visibility);
	SetTableRange(0, type, Register, count, space);
}

void RootParameter::InitalizeAsDescriptorTable(UINT RangeCount, D3D12_SHADER_VISIBILITY visibility)
{
	rootParameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter_.ShaderVisibility = visibility;
	rootParameter_.DescriptorTable.NumDescriptorRanges = RangeCount;
	rootParameter_.DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[RangeCount];
}

void RootParameter::SetTableRange(UINT rangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT Register, UINT count, UINT space)
{
	D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(rootParameter_.DescriptorTable.pDescriptorRanges + rangeIndex);
	range->RangeType = type;
	range->NumDescriptors = count;
	range->BaseShaderRegister = Register;
	range->RegisterSpace = space;
	range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

RootSignature::RootSignature(ID3D12Device* device, UINT numRootParams = 0, UINT numStaticSamplers = 0) : numParameters_(numRootParams)
{
	device_ = device;
	Reset(numRootParams, numStaticSamplers);
}

RootSignature::~RootSignature()
{
}

void RootSignature::Reset(UINT numRootParams, UINT numStaticSamplers)
{
	if (numRootParams > 0) {
		parameterArray_.reset(new RootParameter[numRootParams]);
	}
	else {
		parameterArray_ = nullptr;
		numParameters_ = numRootParams;
	}
	if (numStaticSamplers) {
		samplerArray_.reset(new D3D12_STATIC_SAMPLER_DESC[numStaticSamplers]);
	}
	else {
		samplerArray_ = nullptr;
	}
	numSamplers_ = numStaticSamplers;
	numInitializedStaticSamplers_ = 0;
}

void RootSignature::InitalizeStaticSampler(UINT Register, const D3D12_SAMPLER_DESC& nonStaticSamplerDesc, D3D12_SHADER_VISIBILITY visibility)
{
	assert(numInitializedStaticSamplers_ < numSamplers_);
	D3D12_STATIC_SAMPLER_DESC& staticSamplerDesc = samplerArray_[numInitializedStaticSamplers_++];

	staticSamplerDesc.Filter = nonStaticSamplerDesc.Filter;
	staticSamplerDesc.AddressU = nonStaticSamplerDesc.AddressU;
	staticSamplerDesc.AddressV = nonStaticSamplerDesc.AddressV;
	staticSamplerDesc.AddressW = nonStaticSamplerDesc.AddressW;
	staticSamplerDesc.MipLODBias = nonStaticSamplerDesc.MipLODBias;
	staticSamplerDesc.MaxAnisotropy = nonStaticSamplerDesc.MaxAnisotropy;
	staticSamplerDesc.ComparisonFunc = nonStaticSamplerDesc.ComparisonFunc;
	staticSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
	staticSamplerDesc.MinLOD = nonStaticSamplerDesc.MinLOD;
	staticSamplerDesc.MaxLOD - nonStaticSamplerDesc.MaxLOD;
	staticSamplerDesc.ShaderRegister = Register;
	staticSamplerDesc.RegisterSpace = 0;
	staticSamplerDesc.ShaderVisibility = visibility;

}

void RootSignature::Finalize(D3D12_ROOT_SIGNATURE_FLAGS Flags)
{
	HRESULT result = S_FALSE;
	assert(numInitializedStaticSamplers_ == numSamplers_);

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.NumParameters = numParameters_;
	rootSignatureDesc.pParameters = (const D3D12_ROOT_PARAMETER*)parameterArray_.get();
	rootSignatureDesc.NumStaticSamplers = numSamplers_;
	rootSignatureDesc.pStaticSamplers = (const D3D12_STATIC_SAMPLER_DESC*)samplerArray_.get();
	rootSignatureDesc.Flags = Flags;

	descriptorTableBitMap_ = 0;
	samplerTableBitMap_ = 0;

	
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(result)) {
		assert(false);
	}

	rootSignature_ = nullptr;
	result = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));
}

