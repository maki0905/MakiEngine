#include "DirectXApp.h"
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

DirectXApp::DirectXApp()
{
	// ゲームウィンドウの作成
	win_ = WindowsAPI::GetInstance();
	win_->CreateGameWindow();
	device_ = new Device();
	commandManager_ = new CommandManager(device_->GetDevice());
	swapChain_ = new SwapChain(commandManager_->GetCommandQueue(), device_->GetDxgiFactory());
	renderTarget_ = new RenderTarget(device_->GetDevice(), commandManager_->GetCommandList(), swapChain_->GetSwapChain());
	depth_ = new Depth(device_->GetDevice(), commandManager_->GetCommandList());
	fence_ = new Fence(device_->GetDevice(), commandManager_->GetCommandQueue());
}

DirectXApp::~DirectXApp()
{
	// ゲームウィンドウの破棄
	win_->TerminateGameWindow();
}

void DirectXApp::Run()
{
	while (true) {
		if (win_->ProcessMessage()) {
			break;
		}

		PreDraw();
		PostDraw();

	}
}

void DirectXApp::PreDraw()
{
	commandManager_->BarrierChange(swapChain_->GetSwapChain(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	commandManager_->SetRenderTargets(swapChain_->GetSwapChain(), renderTarget_->GetRtvHandles(), depth_->GetDescriptorHeap());

	// 全画面クリア
	renderTarget_->ClearRenderTarget();
	// 深度バッファクリア
	depth_->ClearDepthView();

	commandManager_->SetViewport(float(WindowsAPI::GetInstance()->kWindowWidth), float(WindowsAPI::GetInstance()->kWindowHeight));

	commandManager_->SetRect(WindowsAPI::GetInstance()->kWindowWidth, WindowsAPI::GetInstance()->kWindowHeight);
}

void DirectXApp::PostDraw()
{
	HRESULT result = S_FALSE;

	commandManager_->BarrierChange(swapChain_->GetSwapChain(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	// 命令のクローズ
	commandManager_->GetCommandList()->Close();

	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { commandManager_->GetCommandList() }; // コマンドリストの配列
	commandManager_->GetCommandQueue()->ExecuteCommandLists(1, cmdLists);

	// バッファをフリップ
	result = swapChain_->GetSwapChain()->Present(1, 0);
#ifdef _DEBUG
	if (FAILED(result)) {
		Microsoft::WRL::ComPtr<ID3D12DeviceRemovedExtendedData> dred;

		result = device_->GetDevice()->QueryInterface(IID_PPV_ARGS(&dred));
		assert(SUCCEEDED(result));

		// 自動パンくず取得
		D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT autoBreadcrumbsOutput{};
		result = dred->GetAutoBreadcrumbsOutput(&autoBreadcrumbsOutput);
		assert(SUCCEEDED(result));
	}
#endif

	fence_->waitForCommandsToFinish();

	commandManager_->CommandClear();

	
}
