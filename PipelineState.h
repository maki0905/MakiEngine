#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <vector>
#include <wrl.h>

class RootSignature;

class PSO
{
public: 
	PSO();

    static void DestroyAll();

    void SetRootSignature(const RootSignature& bindMappings) {
        rootSignature_ = &bindMappings;
    }

    const RootSignature& GetRootSignature() {
        assert(rootSignature_ != nullptr);
        return *rootSignature_;
    }

    ID3D12PipelineState* GetPipelineStateObject() {
        return pso_.Get();
    }


protected:
	const RootSignature* rootSignature_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pso_;
};

class GraphicsPSO : public PSO {
public:
	GraphicsPSO();

    void CreatePSO();

    void SetBlendState(const D3D12_BLEND_DESC& blendDesc);
    void SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc);
    void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
    void SetSampleMask(UINT SampleMask);
    void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType);
    void SetDepthTargetFormat(DXGI_FORMAT DSVFormat, UINT MsaaCount = 1, UINT MsaaQuality = 0);
    void SetRenderTargetFormat(DXGI_FORMAT RTVFormat, DXGI_FORMAT DSVFormat, UINT MsaaCount = 1, UINT MsaaQuality = 0);
    void SetRenderTargetFormats(UINT NumRTVs, const DXGI_FORMAT* RTVFormats, DXGI_FORMAT DSVFormat, UINT MsaaCount = 1, UINT MsaaQuality = 0);
    void SetInputLayout(UINT NumElements, const D3D12_INPUT_ELEMENT_DESC* pInputElementDescs);
    void SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBProps);

private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc_;
	std::shared_ptr<const D3D12_INPUT_ELEMENT_DESC> inputLayouts_;
};
