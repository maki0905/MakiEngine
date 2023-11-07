#include "SwapChain.h"
#include <cassert>
#include "WindowsAPI.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")


SwapChain::SwapChain(ID3D12CommandQueue* commandQueue, IDXGIFactory7* dxgiFactory)
{
	commandQueue_ = commandQueue;
	dxgiFactory_ = dxgiFactory;
	//CreateFactory();
	CreateSwap();
}

SwapChain::~SwapChain()
{
}

HRESULT SwapChain::CreateFactory()
{
	HRESULT result = S_FALSE;
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));
	return result;
}

HRESULT SwapChain::CreateSwap()
{
	HRESULT result = S_FALSE;

	// 各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = WindowsAPI::kWindowWidth;
	swapChainDesc.Height = WindowsAPI::kWindowWidth;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;        // 色情報の書式を一般的なものに
	swapChainDesc.SampleDesc.Count = 1;                       // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;       // バックバッファとして使えるように
	swapChainDesc.BufferCount = 2;                            // バッファ数を２つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は速やかに破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING; // ティアリングサポート
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
	result = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(), WindowsAPI::GetInstance()->GetHwnd(), 
		&swapChainDesc, nullptr, nullptr, &swapChain1
	);
	assert(SUCCEEDED(result));

	// SwapChain4を得る
	swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain_));
	assert(SUCCEEDED(result));
	
	backBufferCount = swapChainDesc.BufferCount;

	return result;
}
