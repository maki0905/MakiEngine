#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>
#include <vector>
#include <string>

class Shader
{
public:
	void DXCInitialize();

	IDxcBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile);

private:
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;


	
	Microsoft::WRL::ComPtr<IDxcBlob> basicVS_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> basicPS_ = nullptr;

};

