#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>
#include <vector>
class Depth
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Depth(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Depth();

	/// <summary>
	/// 深度リソースビューのクリア
	/// </summary>
	/// <returns></returns>
	void ClearDepthView();

	ID3D12DescriptorHeap* GetDescriptorHeap() { return dsvHeap_.Get(); }

private:
	/// <summary>
	/// 深度リソースの生成
	/// </summary>
	/// <returns></returns>
	void CreateDepthResource();

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource();

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

private:
	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;

	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

	// 
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer_;

	// 
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;
};

