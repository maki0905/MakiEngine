#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>
#include <vector>

class RenderTarget
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="swapChain">スワップチェーン</param>
	RenderTarget(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, IDXGISwapChain4* swapChain);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RenderTarget();

	/// <summary>
	/// レンダーターゲットのセット
	/// </summary>
	void SetRender(ID3D12DescriptorHeap* dsvHeap_);

	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// リソースの取得
	/// </summary>
	/// <param name="index">インデックス</param>
	/// <returns></returns>
	ID3D12Resource* GetResource(UINT index) {
		return backBuffers_[index].Get();
	}

	ID3D12DescriptorHeap* GetDescriptorHeap() {
		return rtvDescriptorHeap_.Get();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE* GetRtvHandles() {
		return rtvHandles_;
	}

private:
	/// <summary>
	/// レンダーターゲットの生成
	/// </summary>
	/// <returns></returns>
	HRESULT CreateRenderTargets();

private:
	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;

	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;

	// リソース
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
};

