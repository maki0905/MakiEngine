#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <vector>
#include <wrl.h>

class Device
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Device();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Device();

	ID3D12Device* GetDevice() { return device_.Get(); }
	IDXGIFactory7* GetDxgiFactory() { return dxgiFactory_.Get(); }
private:

	/// <summary>
	/// DXGIデバイス初期化
	/// </summary>
	void InitializeDXGIDevice();

private:
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;

};

