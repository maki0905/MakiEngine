#include "Shader.h"

#include <cassert>

#pragma comment(lib,"dxcompiler.lib")

void Shader::DXCInitialize()
{
	HRESULT result = S_FALSE;

	result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(result));
	result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(result));

	result = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(result));

}

IDxcBlob* Shader::CompileShader(const std::wstring& filePath, const wchar_t* profile)
{
	//hlsl�t�@�C����ǂ�
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//�ǂ߂Ȃ�������~�߂�
	assert(SUCCEEDED(hr));
	//�ǂݍ��񂾃t�@�C���̓��e��ݒ肷��
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; //UTF8�̕����R�[�h�ł��邱�Ƃ�ʒm

	LPCWSTR arguments[] = {
		filePath.c_str(), //�R���p�C���Ώۂ�hlsl�t�@�C����
		L"-E", L"main", //�G���g���[�|�C���g�̎w��B��{�I��main�ȊO�ɂ͂��Ȃ�
		L"-T", profile, //ShaderProfile�̐ݒ�
		L"-Zi", L"-Qembed_debug", //�f�o�b�O�p�̏��𖄂ߍ���
		L"-Od", //�œK�����O���Ă���
		L"-Zpr", //���������C�A�E�g�͍s�D��
	};
	//���ۂ�Shader���R���p�C������
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
		&shaderSourceBuffer, //�ǂݍ��񂾃t�@�C��
		arguments, //�R���p�C���I�v�V����
		_countof(arguments), //�R���p�C���I�v�V�����̐�
		includeHandler_.Get(), //include���܂܂ꂽ���X
		IID_PPV_ARGS(&shaderResult) //�R���p�C������
	);
	//�R���p�C���G���[�ł͂Ȃ�dxc���N���ł��Ȃ��Ȃǒv���I�ȏ�
	assert(SUCCEEDED(hr));

	//�x���E�G���[���o�Ă��烍�O�ɏo���Ď~�߂�
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		//Log(shaderError->GetStringPointer());
		//�x���E�G���[�_���[�b�^�C
		assert(false);
	}

	//�R���p�C�����ʂ�����s�p�̃o�C�i���������擾
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//�����������O���o��
	//Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
	//�����g��Ȃ����\�[�X�����
	shaderSource->Release();
	shaderResult->Release();
	//���s�p�̃o�C�i����ԋp
	return shaderBlob;
}
