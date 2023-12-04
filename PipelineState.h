#pragma once
#include <d3d12.h>
#include <wrl.h>

class RootSignature;

class pipelineState
{
public:
    enum class ShaderType{
        kVS,
        kPS,
    };


public:
	pipelineState(ID3D12Device* device, RootSignature* rootSignature);

	void SetBlendState(const D3D12_BLEND_DESC& blendDesc);
    void SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc);
    void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
    void SetSampleMask(UINT sampleMask);
    void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType);
    void SetDepthTargetFormat(DXGI_FORMAT dsvFormat, UINT msaaCount = 1, UINT msaaQuality = 0);
    void SetRenderTargetFormat(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, UINT msaaCount = 1, UINT msaaQuality = 0);
    void SetRenderTargetFormats(UINT numRTVs, const DXGI_FORMAT* rtvFormats, DXGI_FORMAT dsvFormat, UINT msaaCount = 1, UINT msaaQuality = 0);
    void SetInputLayout(const D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc);
    void SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBProps);
    void SetShader(ShaderType type , Microsoft::WRL::ComPtr<ID3DBlob> blob);

	void Finalize();


protected:
	ID3D12Device* device_;
	RootSignature* rootSignature_;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc_{};

};

