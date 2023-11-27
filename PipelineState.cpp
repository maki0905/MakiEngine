#include "PipelineState.h"
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

static std::map<size_t, Microsoft::WRL::ComPtr<ID3D12PipelineState>> graphicsPSOHashMap;


PSO::PSO()
{
	rootSignature_ = nullptr;
	pso_ = nullptr;
}

void PSO::DestroyAll()
{
	graphicsPSOHashMap.clear();
}


GraphicsPSO::GraphicsPSO()
{
	ZeroMemory(&psoDesc_, sizeof(psoDesc_));
	psoDesc_.NodeMask = 1;
	psoDesc_.SampleMask = 0xFFFFFFFFu;
	psoDesc_.SampleDesc.Count = 1;
	psoDesc_.InputLayout.NumElements = 0;
}

void GraphicsPSO::CreatePSO()
{
	HRESULT result_ = S_FALSE;
	
}

void GraphicsPSO::SetBlendState(const D3D12_BLEND_DESC& blendDesc)
{
	psoDesc_.BlendState = blendDesc;
}

void GraphicsPSO::SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc)
{
	psoDesc_.RasterizerState = rasterizerDesc;
}

void GraphicsPSO::SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc)
{
	psoDesc_.DepthStencilState = depthStencilDesc;
}

void GraphicsPSO::SetSampleMask(UINT SampleMask)
{
	psoDesc_.SampleMask = SampleMask;
}

void GraphicsPSO::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType)
{
	assert(topologyType != D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED);
	psoDesc_.PrimitiveTopologyType = topologyType;
}

void GraphicsPSO::SetDepthTargetFormat(DXGI_FORMAT DSVFormat, UINT MsaaCount, UINT MsaaQuality)
{
	SetRenderTargetFormats(0, nullptr, DSVFormat, MsaaCount, MsaaQuality);
}

void GraphicsPSO::SetRenderTargetFormat(DXGI_FORMAT RTVFormat, DXGI_FORMAT DSVFormat, UINT MsaaCount, UINT MsaaQuality)
{
	SetRenderTargetFormats(1, &RTVFormat, DSVFormat, MsaaCount, MsaaQuality);
}

void GraphicsPSO::SetRenderTargetFormats(UINT NumRTVs, const DXGI_FORMAT* RTVFormats, DXGI_FORMAT DSVFormat, UINT MsaaCount, UINT MsaaQuality)
{
	assert(NumRTVs == 0 || RTVFormats != nullptr);
	for (UINT i = 0; i < NumRTVs; ++i) {
		assert(RTVFormats[i] != DXGI_FORMAT_UNKNOWN);
		psoDesc_.RTVFormats[i] = RTVFormats[i];
	}

	for (UINT i = NumRTVs; i < psoDesc_.NumRenderTargets; ++i) {
		psoDesc_.RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
		psoDesc_.NumRenderTargets = NumRTVs;
		psoDesc_.DSVFormat = DSVFormat;
		psoDesc_.SampleDesc.Count = MsaaCount;
		psoDesc_.SampleDesc.Quality = MsaaQuality;
	}

}

void GraphicsPSO::SetInputLayout(UINT NumElements, const D3D12_INPUT_ELEMENT_DESC* pInputElementDescs)
{
	psoDesc_.InputLayout.NumElements = NumElements;

	if (NumElements > 0) {
		D3D12_INPUT_ELEMENT_DESC* NewElements = (D3D12_INPUT_ELEMENT_DESC*)malloc(sizeof(D3D12_INPUT_ELEMENT_DESC) * NumElements);
		memcpy(NewElements, pInputElementDescs, NumElements * sizeof(D3D12_INPUT_ELEMENT_DESC));
		inputLayouts_.reset((const D3D12_INPUT_ELEMENT_DESC*)NewElements);
	}
	else {
		inputLayouts_ = nullptr;
	}

}

void GraphicsPSO::SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBProps)
{
	psoDesc_.IBStripCutValue = IBProps;
}

