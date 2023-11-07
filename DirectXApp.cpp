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
	HRESULT result = S_FALSE;
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapChain_->GetBackBuffer();

	// リソースバリアを変更（表示状態→描画対象）
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	result = swapChain_->GetSwapChain()->GetBuffer(bbIndex, IID_PPV_ARGS(&barrier.Transition.pResource));
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	commandManager_->GetCommandList()->ResourceBarrier(1, &barrier);

	//renderTarget_->SetRender(depth_->GetDescriptorHeap());

	//UINT bbIndex = swapChain_->GetSwapChain()->GetCurrentBackBufferIndex();
	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH[2];
	rtvH[0] = renderTarget_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	rtvH[1].ptr = rtvH[0].ptr + device_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(depth_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());
	// レンダーターゲットをセット
	commandManager_->GetCommandList()->OMSetRenderTargets(1, &rtvH[bbIndex], false, nullptr);

	// 全画面クリア
	renderTarget_->ClearRenderTarget();
	// 深度バッファクリア
	depth_->ClearDepthView();

	// ビューポートの設定
	D3D12_VIEWPORT viewport =
		D3D12_VIEWPORT(0.0f, 0.0f, float(WindowsAPI::GetInstance()->kWindowWidth), float(WindowsAPI::GetInstance()->kWindowHeight));
	commandManager_->GetCommandList()->RSSetViewports(1, &viewport);
	// シザリング矩形の設定
	D3D12_RECT rect = D3D12_RECT(0, 0, WindowsAPI::GetInstance()->kWindowWidth, WindowsAPI::GetInstance()->kWindowHeight);
	commandManager_->GetCommandList()->RSSetScissorRects(1, &rect);
}

void DirectXApp::PostDraw()
{
	HRESULT result = S_FALSE;

	// リソースバリアを変更（描画対象→表示状態）
	UINT bbIndex = swapChain_->GetSwapChain()->GetCurrentBackBufferIndex();
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	result = swapChain_->GetSwapChain()->GetBuffer(bbIndex, IID_PPV_ARGS(&barrier.Transition.pResource));
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandManager_->GetCommandList()->ResourceBarrier(1, &barrier);

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

	commandManager_->GetCommandAllocator()->Reset(); // キューをクリア
	commandManager_->GetCommandList()->Reset(commandManager_->GetCommandAllocator(),
		nullptr); // 再びコマンドリストを貯める準備
}
