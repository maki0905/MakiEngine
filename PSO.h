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
protected:
	const wchar_t name_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pso_;
};

class GraphicsPSO : public PSO {
public:

private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc_;
	std::shared_ptr<const D3D12_INPUT_ELEMENT_DESC> inputLayouts_;
};
